#include "link_parser_adapter_impl.hpp"

#include <clocale>
#include <cstring>

#include "exceptions.hpp"
#include "logging.hpp"


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
    int verbosity = psi_logger.getLoggingPriority() / 100 - 4;
    parse_options_set_verbosity(parseOptions, verbosity);
    starts_.clear();
    ends_.clear();
    edgeDescriptions_.clear();
    freeSentence();
    sentence_ = sentence_create(sentenceStr.c_str(), dictionary_);
    if (!sentence_) {
        throw ParserException("Could not process sentence: " + sentenceStr);
    }
    if (sentence_parse(sentence_, parseOptions)) {

        size_t currentPos = 0;
        size_t foundPos = 0;
        int wordNo = 0;
        while (wordNo < sentence_length(sentence_)) {
            const char * word = sentence_get_word(sentence_, wordNo);
            foundPos = sentenceStr.find(word, currentPos);
            if (foundPos != std::string::npos) {
                starts_[wordNo] = foundPos;
                ends_[wordNo] = currentPos = foundPos + strlen(word);
            }
            ++wordNo;
        }

        Linkage linkage = linkage_create(0, sentence_, parseOptions);
        CNode * ctree = linkage_constituent_tree(linkage);
        extractEdgeDescriptions(ctree, linkage);
        linkage_free_constituent_tree(ctree);
        linkage_delete(linkage);

    }
    return edgeDescriptions_;
}


int LinkParserAdapterImpl::getNextNumber_() {
    return number_++;
}


int LinkParserAdapterImpl::extractEdgeDescriptions(CNode * ctree, Linkage linkage) {
    int id = -1;
    if (ctree) {
        std::list<int> children;
        for (
            CNode * subtree = linkage_constituent_node_get_child(ctree);
            subtree != NULL;
            subtree = linkage_constituent_node_get_next(subtree)
        ) {
            children.push_back(extractEdgeDescriptions(subtree, linkage));
        }
        id = getNextNumber_();
        int start = linkage_constituent_node_get_start(ctree) + 1;
        int end = linkage_constituent_node_get_end(ctree) + 1;
        std::string label;
        const char * grammarCategory = linkage_constituent_node_get_label(ctree);
        const char * wordPOS = linkage_get_word(linkage, start);
        if (children.empty() && start == end && wordPOS) {
            label = wordPOS;
        } else if (grammarCategory) {
            label = grammarCategory;
        } else {
            label = "∅";
        }
        edgeDescriptions_.insert(std::pair<int, EdgeDescription>(
            id,
            EdgeDescription(id, starts_[start], ends_[end], label, children)
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
