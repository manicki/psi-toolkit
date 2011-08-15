#ifndef DESCRIPTION_OUTPUT_ITERATOR_HDR
#define DESCRIPTION_OUTPUT_ITERATOR_HDR

#include "lattice.hpp"

/**
 * Lemmatizers return the results of lemmatization using
 * an lemmatize output iterators.
 */
class LemmatizerOutputIterator {
public:
    void addCorrection(
        const std::string correction,
        Lattice::Score score,
        int ruleId);

    void addNormalization(
        const std::string normalization,
        Lattice::Score score,
        int ruleId);

    void addLexeme(
        const AnnotationItem& item,
        Lattice::Score score,
        int ruleId);
    
    void addForm(
        const AnnotationItem& item,
        Lattice::Score score,
        int ruleId);
private:
    virtual void doAddCorrection(
        const std::string correction,
        Lattice::Score score,
        int ruleId);

    virtual void doAddNormalization(
        const std::string normalization,
        Lattice::Score score,
        int ruleId);

    virtual void doAddLexeme(
        const AnnotationItem& item,
        Lattice::Score score,
        int ruleId);
    
    virtual void doAddForm(
        const AnnotationItem& item,
        Lattice::Score score,
        int ruleId);
};

#endif
