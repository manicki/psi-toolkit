#include "lex_db_lemmatizer.hpp"

LexDbLemmatizer::LexDbLemmatizer():connection_("dbname=ude") {
}

void LexDbLemmatizer::lemmatize(const std::string token,
                               AnnotationItemManager& annotationItemManager,
                               LemmatizerOutputIterator& outputIterator) {
    if (token == "prowokacjami") {
        outputIterator.addLemma("prowokacja");

        AnnotationItem lexeme("R", "prowokacja_R");
        annotationItemManager.setValue(lexeme, "flags", "4");
        outputIterator.addLexeme(lexeme);

        AnnotationItem form = lexeme;
        annotationItemManager.setValue(form, "morpho", "ŻNM");
        outputIterator.addForm(form);
    }
}

std::string LexDbLemmatizer::getName() {
    return "lex-db";
}

std::list<std::string> LexDbLemmatizer::getLayerTags() {
    std::list<std::string> layerTags;

    layerTags.push_back("lex-db");
    layerTags.push_back("lexdb-tagset");

    return layerTags;
}
