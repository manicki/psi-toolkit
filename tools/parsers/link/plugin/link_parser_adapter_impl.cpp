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
    Sentence sentence = sentence_create(sentenceStr.c_str(), dictionary_);
    sentence_delete(sentence);
    return "parsed by LINK";
}


// ==============================================

extern "C" LinkParserAdapterImpl * create() {
    return new LinkParserAdapterImpl;
}

extern "C" void destroy(LinkParserAdapterImpl * Tl) {
    delete Tl;
}
