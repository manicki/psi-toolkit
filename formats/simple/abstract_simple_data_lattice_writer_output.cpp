#include <iostream>
#include <list>
#include <string>

#include "logging.hpp"

#include "abstract_simple_data_lattice_writer_output.hpp"

typedef std::pair<std::string, std::string> string_pair;

AbstractSimpleDataLatticeWriterOutput::AbstractSimpleDataLatticeWriterOutput() {
    latticeAnnotationItemManager_ =  NULL;
}

void AbstractSimpleDataLatticeWriterOutput::push(const std::string & textElement) {
    pushToCurrentArrayPointer_(textElement);
}

void AbstractSimpleDataLatticeWriterOutput::push(
     const AnnotationItem & element,
     AnnotationItemManager * latticeAnnotationItemManager) {
    pushToCurrentArrayPointer_(element, latticeAnnotationItemManager);
}

void AbstractSimpleDataLatticeWriterOutput::openNewSubArray() {
    pushCurrentArrayPointerOnStack_();
    newCurrentArrayPointer_();
}

void AbstractSimpleDataLatticeWriterOutput::closeSubArray() {
    closeSubArray_(false);
}

void AbstractSimpleDataLatticeWriterOutput::closeSubArrayWithFlattenOneElement() {
    closeSubArray_(true);
}

void AbstractSimpleDataLatticeWriterOutput::closeSubArray_(bool flattenOneElement) {
    if ( !arraysStack_.empty() ) {
        ArrayPointer parentArrayPointer = arraysStack_.top();
        arraysStack_.pop();

        ReferencePointer currentArrayReference = getCurrentArrayReference_();

        if ( !isCurrentArrayEmpty_()){
            if (flattenOneElement) {
                currentArrayReference = tryToFlattenOneElementCurrentArray();
            }

            if (currentArrayReference) {
                pushToArrayPointer_(parentArrayPointer, currentArrayReference);
            }
        }

        currentArrayPointer_ = parentArrayPointer;
    }
}

bool AbstractSimpleDataLatticeWriterOutput::isCurrentArrayEmpty_() {
    return 0 == getCurrentArrayLength_();
}


ReferencePointer AbstractSimpleDataLatticeWriterOutput::tryToFlattenOneElementCurrentArray() {

    long arrayLength = getCurrentArrayLength_();

    if ( 1 == arrayLength) {
        return doFlattenOneElementCurrentArray();
    } else {
        return getCurrentArrayReference_();
    }
}



