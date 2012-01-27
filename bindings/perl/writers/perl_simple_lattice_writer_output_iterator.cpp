#include "perl_simple_lattice_writer_output_iterator.hpp"

#include "logging.hpp"

PerlSimpleLatticeWriterOutputIterator::PerlSimpleLatticeWriterOutputIterator(
  PerlLatticeWriterOutput & output,
  bool withArgs
                                                                             
) :
    output_(output),
    withArgs_(withArgs)
{
}

PerlSimpleLatticeWriterOutputIterator::~PerlSimpleLatticeWriterOutputIterator() {
}

void PerlSimpleLatticeWriterOutputIterator::doPutElement(const AnnotationItem& element) {

    if(this->withArgs_) {
        output_.push(element);
    } else {
        output_.push(getElementAnnotationItemStringToPut(element));
    }
}

void PerlSimpleLatticeWriterOutputIterator::doOpenAlternative() {
    output_.openNewSubArray();
}

void PerlSimpleLatticeWriterOutputIterator::doCloseAlternative() {
    output_.closeSubArrayWithFlattenOneElement();
}

void PerlSimpleLatticeWriterOutputIterator::doOpenGroup(const std::string& /*group*/) {
    output_.openNewSubArray();
}

void PerlSimpleLatticeWriterOutputIterator::doCloseGroup(const std::string& /*group*/) {
    output_.closeSubArray();
}

void PerlSimpleLatticeWriterOutputIterator::doFlush() {
}
