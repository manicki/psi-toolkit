#include "fake_lemmatizer.hpp"

FakeLemmatizer::FakeLemmatizer() {
}

void FakeLemmatizer::lemmatize(const std::string token,
                               AnnotationItemManager& annotationItemManager,
                               LemmatizerOutputIterator& outputIterator) {
    if (token == "prowokacjami") {
        AnnotationItem lemma("'$prowokacja'");
        annotationItemManager.setValue(lemma, "pos", "R");
        annotationItemManager.setValue(lemma, "flags", "4");
        outputIterator.addLemma(lemma);

        AnnotationItem form = lemma;
        annotationItemManager.setValue(lemma, "morpho", "ŻNM");
        outputIterator.addForm(form);
    }
}

std::string FakeLemmatizer::getName() {
    return "lex-fake";
}
