#include "link_parser_adapter_impl.hpp"

#include <clocale>
#include <cstring>

#include "exceptions.hpp"


LinkParserAdapterImpl::LinkParserAdapterImpl() : dictionary_(NULL), sentence_(NULL), number_(0) {
    setlocale(LC_ALL, "");
}


LinkParserAdapterImpl::~LinkParserAdapterImpl() {
    freeSentence();
    freeDictionary();
}


void LinkParserAdapterImpl::setDictionary(std::string language) {
    freeDictionary();
    dictionary_ = dictionary_create_lang(language.c_str());
    if (!dictionary_) {
        throw ParserException("Could not create dictionary for language: " + language);
    }
}


void LinkParserAdapterImpl::setDictionary(
    std::string dictionaryName,
    std::string postProcessFileName,
    std::string constituentKnowledgeName,
    std::string affixName
) {
    freeDictionary();
    dictionary_ = dictionary_create(
        dictionaryName.c_str(),
        postProcessFileName.empty() ? NULL : postProcessFileName.c_str(),
        constituentKnowledgeName.empty() ? NULL : constituentKnowledgeName.c_str(),
        affixName.empty() ? NULL : affixName.c_str()
    );
    if (!dictionary_) {
        throw ParserException(
            "Could not create dictionary from files: " + dictionaryName +
            ", " + postProcessFileName +
            ", " + constituentKnowledgeName +
            ", " + affixName
        );
    }
}


std::map<int, EdgeDescription> LinkParserAdapterImpl::parseSentence(std::string sentenceStr) {
    Parse_Options parseOptions = parse_options_create();
    freeSentence();
    sentence_ = sentence_create(sentenceStr.c_str(), dictionary_);
    if (!sentence_) {
        throw ParserException("Could not process sentence: " + sentenceStr);
    }
    if (sentence_parse(sentence_, parseOptions)) {

        size_t pos = 0;
        int wordNo = 0;
        while (wordNo < sentence_length(sentence_)) {
            const char * word = sentence_get_word(sentence_, wordNo);
            pos = sentenceStr.find(word, pos);
            if (pos == std::string::npos) {
                pos = 0;
            }
            starts_[wordNo] = pos;
            pos += strlen(word);
            ends_[wordNo] = pos;
            ++wordNo;
        }

        Linkage linkage = linkage_create(0, sentence_, parseOptions);
        CNode * ctree = linkage_constituent_tree(linkage);
        extractEdgeDescriptions(ctree);
        linkage_free_constituent_tree(ctree);
        linkage_delete(linkage);

    }
    return edgeDescriptions_;
}


int LinkParserAdapterImpl::getNextNumber_() {
    return number_++;
}


int LinkParserAdapterImpl::extractEdgeDescriptions(CNode * ctree) {
    int id = -1;
    if (ctree) {
        std::list<int> children;
        for (
            CNode * subtree = linkage_constituent_node_get_child(ctree);
            subtree != NULL;
            subtree = linkage_constituent_node_get_next(subtree)
        ) {
            children.push_back(extractEdgeDescriptions(subtree));
        }
        id = getNextNumber_();
        int start = starts_[linkage_constituent_node_get_start(ctree) + 1];
        int end = ends_[linkage_constituent_node_get_end(ctree) + 1];
        const char * label = linkage_constituent_node_get_label(ctree);
        if (!label) {
            label = "∅";
        }
        edgeDescriptions_.insert(std::pair<int, EdgeDescription>(
            id,
            EdgeDescription(id, start, end, label, children)
        ));
    }
    return id;
}


void LinkParserAdapterImpl::freeDictionary() {
    if (dictionary_) {
        dictionary_delete(dictionary_);
        dictionary_ = NULL;
    }
}


void LinkParserAdapterImpl::freeSentence() {
    if (sentence_) {
        sentence_delete(sentence_);
        sentence_ = NULL;
    }
}


// ==============================================

extern "C" LinkParserAdapterImpl * create() {
    return new LinkParserAdapterImpl;
}

extern "C" void destroy(LinkParserAdapterImpl * Tl) {
    delete Tl;
}
