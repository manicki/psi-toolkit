#include "json_lattice_writer_output_iterator.hpp"

#include "logging.hpp"

JSONLatticeWriterOutputIterator::JSONLatticeWriterOutputIterator(
                                                                 std::ostream & output,
                                                                 bool withArgs)
    : 
    outputWrapper_(output),
    withArgs_(withArgs)
{
    
}

JSONLatticeWriterOutputIterator::~JSONLatticeWriterOutputIterator() {
}

void JSONLatticeWriterOutputIterator::doPutElement(const AnnotationItem& element) {

    if (withArgs_) {
        outputWrapper_.push(element, getLatticeAnnotationItemManagerPointer());
    } else {
        outputWrapper_.push(getElementAnnotationItemStringToPut(element));
    }
}

void JSONLatticeWriterOutputIterator::doOpenAlternative() {
    outputWrapper_.openNewSubArray();
}

void JSONLatticeWriterOutputIterator::doCloseAlternative() {
    outputWrapper_.closeSubArrayWithFlattenOneElement();
}

void JSONLatticeWriterOutputIterator::doOpenGroup(const std::string& /*group*/) {
    outputWrapper_.openNewSubArray();
}

void JSONLatticeWriterOutputIterator::doCloseGroup(const std::string& /*group*/) {
    outputWrapper_.closeSubArray();
}

void JSONLatticeWriterOutputIterator::doFlush() {
}

