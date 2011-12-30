#include "perl_simple_lattice_writer_output_iterator.hpp"

#include "logging.hpp"

PerlSimpleLatticeWriterOutputIterator::PerlSimpleLatticeWriterOutputIterator(
    PerlLatticeWriterOutput & output
) :
    output_(output)
{
}

PerlSimpleLatticeWriterOutputIterator::~PerlSimpleLatticeWriterOutputIterator() {
}

void PerlSimpleLatticeWriterOutputIterator::doPutElement(const std::string& element) {
    output_.push(element);
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
