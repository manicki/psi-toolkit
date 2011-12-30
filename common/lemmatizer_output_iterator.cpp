#include "lemmatizer_output_iterator.hpp"

void LemmatizerOutputIterator::addCorrection(
        const std::string& correction,
        Lattice::Score score,
        int ruleId) {
    doAddCorrection(correction, score, ruleId);
}

void LemmatizerOutputIterator::addNormalization(
    const std::string& normalization,
    Lattice::Score score,
    int ruleId) {
    doAddNormalization(normalization, score, ruleId);
}

void LemmatizerOutputIterator::addLemma(
    const std::string& lemma,
    Lattice::Score score,
    int ruleId) {
    doAddLemma(lemma, score, ruleId);
}

void LemmatizerOutputIterator::addLexeme(
    const AnnotationItem& item,
    Lattice::Score score,
    int ruleId) {
    doAddLexeme(item, score, ruleId);
}

void LemmatizerOutputIterator::addForm(
    const AnnotationItem& item,
    Lattice::Score score,
    int ruleId) {
    doAddForm(item, score, ruleId);
}

LemmatizerOutputIterator::~LemmatizerOutputIterator() {
}
