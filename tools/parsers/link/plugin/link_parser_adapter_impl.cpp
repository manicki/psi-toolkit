#include "link_parser_adapter_impl.hpp"


LinkParserAdapterImpl::LinkParserAdapterImpl() : dictionary_(NULL) { }


LinkParserAdapterImpl::~LinkParserAdapterImpl() {
    if (dictionary_) {
        dictionary_delete(dictionary_);
    }
}


void LinkParserAdapterImpl::setDictionary(std::string filename) {
    if (dictionary_) {
        dictionary_delete(dictionary_);
    }
    dictionary_ = dictionary_create(filename.c_str(), NULL, NULL, NULL);
}


std::string LinkParserAdapterImpl::parseSentence(std::string sentenceStr) {
    std::string result;
    Parse_Options parseOptions = parse_options_create();
    Sentence sentence = sentence_create(sentenceStr.c_str(), dictionary_);
    if (sentence_parse(sentence, parseOptions)) {
        Linkage linkage = linkage_create(0, sentence, parseOptions);
        CNode_s * ctree = linkage_constituent_tree(linkage);
        if (ctree) {
            if (linkage_constituent_node_get_label(ctree)) {
                result = linkage_constituent_node_get_label(ctree);
            } else {
                result = "NO LABEL!";
            }
        }
/*
        char * ctreePrint = linkage_print_constituent_tree(linkage, 1);
        if (ctreePrint) {
            result = ctreePrint;
        } else {
            result = "PARSING FAILED";
        }
        delete ctreePrint;
*/
        linkage_free_constituent_tree(ctree);
        linkage_delete(linkage);
    }
    sentence_delete(sentence);
    return result;
}


// ==============================================

extern "C" LinkParserAdapterImpl * create() {
    return new LinkParserAdapterImpl;
}

extern "C" void destroy(LinkParserAdapterImpl * Tl) {
    delete Tl;
}
