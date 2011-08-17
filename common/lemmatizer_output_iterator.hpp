#ifndef DESCRIPTION_OUTPUT_ITERATOR_HDR
#define DESCRIPTION_OUTPUT_ITERATOR_HDR

#include <string>

#include "annotation_item.hpp"
#include "lattice.hpp"

/**
 * Lemmatizers return the results of lemmatization using
 * an lemmatize output iterators.
 */
class LemmatizerOutputIterator {
public:
    // usually spell checking correction
    void addCorrection(
        const std::string& correction,
        Lattice::Score score=0,
        int ruleId=-1);

    // usually truecasing
    void addNormalization(
        const std::string& normalization,
        Lattice::Score score=0,
        int ruleId=-1);

    void addLemma(
        const std::string& item,
        Lattice::Score score=0,
        int ruleId=-1);

    void addLexeme(
        const AnnotationItem& item,
        Lattice::Score score=0,
        int ruleId=-1);

    void addForm(
        const AnnotationItem& item,
        Lattice::Score score=0,
        int ruleId=-1);

    virtual ~LemmatizerOutputIterator();

private:
    virtual void doAddCorrection(
        const std::string& correction,
        Lattice::Score score,
        int ruleId) = 0;

    virtual void doAddNormalization(
        const std::string& normalization,
        Lattice::Score score,
        int ruleId) = 0;

    virtual void doAddLemma(
        const std::string& lemma,
        Lattice::Score score,
        int ruleId) = 0;

    virtual void doAddLexeme(
        const AnnotationItem& item,
        Lattice::Score score,
        int ruleId) = 0;

    virtual void doAddForm(
        const AnnotationItem& item,
        Lattice::Score score,
        int ruleId) = 0;
};

#endif
