#include <iostream>
#include <list>
#include <string>

#include "logging.hpp"

#include "perl_lattice_writer_output.hpp"

typedef std::pair<std::string, std::string> string_pair;

PerlLatticeWriterOutput::PerlLatticeWriterOutput(Sink &arrayPointer) {
    latticeAnnotationItemManager_ =  NULL;
    
    currentArrayPointer_ = arrayPointer;
}

void PerlLatticeWriterOutput::push(const std::string & textElement) {
    SV * element_sv = newSVpv(textElement.c_str(), 0);
    av_push( currentArrayPointer_, element_sv);
}

void PerlLatticeWriterOutput::push(
     const AnnotationItem & element,
     AnnotationItemManager * latticeAnnotationItemManager) {

    HV * newHashEntry = newHV();
    SV * categoryValue = newSVpv(element.getCategory().c_str(), 0);
    SV * textValue = newSVpv(element.getText().c_str(), 0);

    hv_store(newHashEntry,
             "category", 8,
             categoryValue, 0);
    hv_store(newHashEntry,
             "text", 4,
             textValue, 0);

    HV * itemValuesHash = newHV();
    
    std::list< std::pair<std::string, std::string> > values =
        latticeAnnotationItemManager->getValues(element);

    BOOST_FOREACH(string_pair currentValue, values) {
        SV * currentValueScalar = newSVpv(currentValue.second.c_str(), 0);
        hv_store(itemValuesHash,
                 currentValue.first.c_str(), currentValue.first.size(),
                 currentValueScalar, 0);
    }

    hv_store(newHashEntry,
             "values", 6,
             newRV_inc((SV *)itemValuesHash), 0);

    SV * newHashEntryReference = newRV_inc((SV *) newHashEntry);
    av_push( currentArrayPointer_, newHashEntryReference);

}

void PerlLatticeWriterOutput::openNewSubArray() {
    arraysStack_.push(currentArrayPointer_);
    currentArrayPointer_ = newAV();
}

void PerlLatticeWriterOutput::closeSubArray() {
    closeSubArray_(false);
}

void PerlLatticeWriterOutput::closeSubArrayWithFlattenOneElement() {
    closeSubArray_(true);
}

void PerlLatticeWriterOutput::closeSubArray_(bool flattenOneElement) {
    if ( !arraysStack_.empty() ) {
        AV * parentArrayPointer = arraysStack_.top();
        arraysStack_.pop();

        SV * currentArrayReference = getCurrentArrayReference_();

        if ( !isCurrentArrayEmpty_()){
            if (flattenOneElement) {
                currentArrayReference = tryToFlattenOneElementCurrentArray();
            }

            if (currentArrayReference) {
                av_push(parentArrayPointer, currentArrayReference);
            }
        }
        currentArrayPointer_ = parentArrayPointer;
    }
}

bool PerlLatticeWriterOutput::isCurrentArrayEmpty_() {
    return 0 == getCurrentArrayLength_();
}


long PerlLatticeWriterOutput::getCurrentArrayLength_() {
    long arrayLength = av_len(currentArrayPointer_) + 1;
    return arrayLength;
}

SV * PerlLatticeWriterOutput::tryToFlattenOneElementCurrentArray() {

    long arrayLength = av_len(currentArrayPointer_) + 1;

    if ( 1 == arrayLength) {
        SV * elementReference = av_pop(currentArrayPointer_);
        av_undef(currentArrayPointer_);
        return elementReference;
    } else {
        return getCurrentArrayReference_();
    }
}


SV * PerlLatticeWriterOutput::getCurrentArrayReference_() {
    return newRV_inc((SV*) currentArrayPointer_);
}
