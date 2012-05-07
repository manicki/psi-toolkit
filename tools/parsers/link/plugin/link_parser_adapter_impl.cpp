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
        result = linkage_print_diagram(linkage);
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
