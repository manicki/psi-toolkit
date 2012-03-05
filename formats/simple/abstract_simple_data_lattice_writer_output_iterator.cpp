#include "abstract_simple_data_lattice_writer_output_iterator.hpp"

#include "logging.hpp"


AbstractSimpleDataLatticeWriterOutputIterator::AbstractSimpleDataLatticeWriterOutputIterator(
  AbstractSimpleDataLatticeWriterOutput & output,
  bool withArgs) :
    output_(output),
    withArgs_(withArgs)
{
}

AbstractSimpleDataLatticeWriterOutputIterator::~AbstractSimpleDataLatticeWriterOutputIterator() {
}

void AbstractSimpleDataLatticeWriterOutputIterator::doPutElement(const AnnotationItem& element) {

    if (withArgs_) {
        output_.push(element, getLatticeAnnotationItemManagerPointer());
    } else {
        output_.push(getElementAnnotationItemStringToPut(element));
    }
}

void AbstractSimpleDataLatticeWriterOutputIterator::doOpenAlternative() {
    output_.openNewSubArray();
}

void AbstractSimpleDataLatticeWriterOutputIterator::doCloseAlternative() {
    output_.closeSubArrayWithFlattenOneElement();
}

void AbstractSimpleDataLatticeWriterOutputIterator::doOpenGroup(const std::string& /*group*/) {
    output_.openNewSubArray();
}

void AbstractSimpleDataLatticeWriterOutputIterator::doCloseGroup(const std::string& /*group*/) {
    output_.closeSubArray();
}

void AbstractSimpleDataLatticeWriterOutputIterator::doFlush() {
}


