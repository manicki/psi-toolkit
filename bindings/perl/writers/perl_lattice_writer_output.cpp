#include <iostream>
#include "logging.hpp"

#include "perl_lattice_writer_output.hpp"

PerlLatticeWriterOutput::PerlLatticeWriterOutput(AV * arrayPointer)
    : currentArrayPointer_(arrayPointer) {
}

void PerlLatticeWriterOutput::push(const std::string & textElement) {

    SV * element_sv = newSVpv(textElement.c_str(), 0);
    av_push( currentArrayPointer_, element_sv);
}

void PerlLatticeWriterOutput::push(const AnnotationItem & element) {
    // TODO
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
        PerlArrayPointer parentArrayPointer = arraysStack_.top();
        arraysStack_.pop();

        PerlReference currentArrayReference = getCurrentArrayReference_();

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

PerlReference PerlLatticeWriterOutput::tryToFlattenOneElementCurrentArray() {

    long arrayLength = av_len(currentArrayPointer_) + 1;

    if ( 1 == arrayLength) {
        PerlReference elementReference = av_pop(currentArrayPointer_);
        av_undef(currentArrayPointer_);
        return elementReference;
    } else {
        return getCurrentArrayReference_();
    }
}


PerlReference PerlLatticeWriterOutput::getCurrentArrayReference_() {
    return newRV_inc((PerlReference) currentArrayPointer_);
}
