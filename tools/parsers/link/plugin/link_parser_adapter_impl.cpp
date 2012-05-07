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
        result = extractEdgeDescriptions(ctree);
        linkage_free_constituent_tree(ctree);
        linkage_delete(linkage);

    }
    return result;
}


std::vector<EdgeDescription> LinkParserAdapterImpl::extractEdgeDescriptions(CNode * ctree) {
    std::vector<EdgeDescription> result;
    if (ctree) {
        int start = starts_[linkage_constituent_node_get_start(ctree) + 1];
        int end = ends_[linkage_constituent_node_get_end(ctree) + 1];
        const char * label = linkage_constituent_node_get_label(ctree);
        CNode * next = linkage_constituent_node_get_next(ctree);
        CNode * child = linkage_constituent_node_get_child(ctree);
        std::vector<EdgeDescription> nextDescs = extractEdgeDescriptions(next);
        std::vector<EdgeDescription> childDescs = extractEdgeDescriptions(child);
        result.insert(result.end(), nextDescs.begin(), nextDescs.end());
        result.insert(result.end(), childDescs.begin(), childDescs.end());
        std::list<int> rsiblings;
        if (!nextDescs.empty()) {
            rsiblings = nextDescs.back().rsiblings;
        }
        rsiblings.push_front(result.size());
        std::list<int> children;
        if (!childDescs.empty()) {
            children = childDescs.back().rsiblings;
        }
        if (label) {
            result.push_back(EdgeDescription(start, end, label, children, rsiblings));
        } else {
            result.push_back(EdgeDescription(start, end, "NOLABEL", children, rsiblings));
        }
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
