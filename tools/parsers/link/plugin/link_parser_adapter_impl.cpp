#include "link_parser_adapter_impl.hpp"

#include <cstring>


LinkParserAdapterImpl::LinkParserAdapterImpl() : dictionary_(NULL), sentence_(NULL) { }


LinkParserAdapterImpl::~LinkParserAdapterImpl() {
    if (dictionary_) {
        dictionary_delete(dictionary_);
    }
    if (sentence_) {
        sentence_delete(sentence_);
    }
}


void LinkParserAdapterImpl::setDictionary(std::string filename) {
    if (dictionary_) {
        dictionary_delete(dictionary_);
    }
    dictionary_ = dictionary_create(filename.c_str(), NULL, NULL, NULL);
}


std::vector<EdgeDescription> LinkParserAdapterImpl::parseSentence(std::string sentenceStr) {
    std::vector<EdgeDescription> result;
    Parse_Options parseOptions = parse_options_create();
    if (sentence_) {
        sentence_delete(sentence_);
    }
    sentence_ = sentence_create(sentenceStr.c_str(), dictionary_);
    if (sentence_parse(sentence_, parseOptions)) {

        size_t pos = 0;
        int wordNo = 0;
        while (pos != std::string::npos && wordNo < sentence_length(sentence_)) {
            const char * word = sentence_get_word(sentence_, wordNo);
            pos = sentenceStr.find(word, pos);
            starts_[wordNo] = pos;
            ends_[wordNo] = pos + strlen(word);
            ++wordNo;
        }

        Linkage linkage = linkage_create(0, sentence_, parseOptions);
        CNode * ctree = linkage_constituent_tree(linkage);
        result = extractEdgeDescriptions(ctree);
        linkage_free_constituent_tree(ctree);
        linkage_delete(linkage);

    }
    return result;
}


std::vector<EdgeDescription> LinkParserAdapterImpl::extractEdgeDescriptions(CNode * ctree) {
    std::vector<EdgeDescription> result;
    if (ctree) {
        int start = starts_[linkage_constituent_node_get_start(ctree)];
        int end = ends_[linkage_constituent_node_get_end(ctree)];
        const char * label = linkage_constituent_node_get_label(ctree);
        CNode * next = linkage_constituent_node_get_next(ctree);
        CNode * child = linkage_constituent_node_get_child(ctree);
        std::vector<EdgeDescription> nextDesc = extractEdgeDescriptions(next);
        std::vector<EdgeDescription> childDesc = extractEdgeDescriptions(child);
        int count = result.size() + nextDesc.size() + childDesc.size();
        std::vector<int> children;
        children.push_back(-1);
        if (label) {
            result.push_back(EdgeDescription(count, start, end, label, children));
        } else {
            result.push_back(EdgeDescription(count, start, end, "NOLABEL", children));
        }
        result.insert(result.end(), nextDesc.begin(), nextDesc.end());
        result.insert(result.end(), childDesc.begin(), childDesc.end());
    }
    return result;
}


// ==============================================

extern "C" LinkParserAdapterImpl * create() {
    return new LinkParserAdapterImpl;
}

extern "C" void destroy(LinkParserAdapterImpl * Tl) {
    delete Tl;
}
