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
}

void PerlSimpleLatticeWriterOutputIterator::doCloseAlternative() {
}

void PerlSimpleLatticeWriterOutputIterator::doOpenGroup(const std::string& group) {
}

void PerlSimpleLatticeWriterOutputIterator::doCloseGroup(const std::string& group) {
}

void PerlSimpleLatticeWriterOutputIterator::doFlush() {
}
