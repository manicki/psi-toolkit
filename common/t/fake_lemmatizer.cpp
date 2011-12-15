#include "fake_lemmatizer.hpp"

FakeLemmatizer::FakeLemmatizer(const boost::program_options::variables_map&) {
}

void FakeLemmatizer::lemmatize(const std::string& token,
                               AnnotationItemManager& annotationItemManager,
                               LemmatizerOutputIterator& outputIterator) {
    if (token == "prowokacjami") {
        outputIterator.addLemma("prowokacja");

        AnnotationItem lexeme("R", StringFrag("prowokacja_R"));
        annotationItemManager.setValue(lexeme, "flags", "4");
        outputIterator.addLexeme(lexeme);

        AnnotationItem form = lexeme;
        annotationItemManager.setValue(form, "morpho", "ŻNM");
        outputIterator.addForm(form);
    }
}

boost::program_options::options_description FakeLemmatizer::optionsHandled() {
    boost::program_options::options_description desc;

    return desc;
}


std::string FakeLemmatizer::getName() {
    return "lex-fake";
}

std::list<std::string> FakeLemmatizer::getLayerTags() {
    std::list<std::string> layerTags;

    layerTags.push_back("lex-fake");
    layerTags.push_back("lexdb-tagset");

    return layerTags;
}
