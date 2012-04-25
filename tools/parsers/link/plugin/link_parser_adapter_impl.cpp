#include "link_parser_adapter_impl.hpp"


LinkParserAdapterImpl::LinkParserAdapterImpl() {
    dictionary_ = dictionary_create("%ITSDATA%/%LANG%/rules.%LANG%", NULL, NULL, NULL);
}


LinkParserAdapterImpl::~LinkParserAdapterImpl() {
    if (dictionary_) {
        dictionary_delete(dictionary_);
    }
}


std::string LinkParserAdapterImpl::parseSentence(std::string sentenceStr) {
    // Sentence sentence = sentence_create(sentenceStr.c_str(), dictionary_);
    // sentence_delete(sentence);
    return "parsed by LINK";
}


// ==============================================

extern "C" LinkParserAdapterImpl * create() {
    return new LinkParserAdapterImpl;
}

extern "C" void destroy(LinkParserAdapterImpl * Tl) {
    delete Tl;
}
