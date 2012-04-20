#include "perl_simple_lattice_writer_output_iterator.hpp"

#if HAVE_PERL_BINDINGS

#include "logging.hpp"

PerlSimpleLatticeWriterOutputIterator::PerlSimpleLatticeWriterOutputIterator(
  Sink & output,
  bool withArgs)
    :
    outputWrapper_(output),
    withArgs_(withArgs)
{
}

PerlSimpleLatticeWriterOutputIterator::~PerlSimpleLatticeWriterOutputIterator() {
}

void PerlSimpleLatticeWriterOutputIterator::doPutElement(const AnnotationItem& element) {

    if (withArgs_) {
        outputWrapper_.push(element, getLatticeAnnotationItemManagerPointer());
    } else {
        outputWrapper_.push(getElementAnnotationItemStringToPut(element));
    }
}

void PerlSimpleLatticeWriterOutputIterator::doOpenAlternative() {
    outputWrapper_.openNewSubArray();
}

void PerlSimpleLatticeWriterOutputIterator::doCloseAlternative() {
    outputWrapper_.closeSubArrayWithFlattenOneElement();
}

void PerlSimpleLatticeWriterOutputIterator::doOpenGroup(const std::string& /*group*/) {
    outputWrapper_.openNewSubArray();
}

void PerlSimpleLatticeWriterOutputIterator::doCloseGroup(const std::string& /*group*/) {
    outputWrapper_.closeSubArray();
}

void PerlSimpleLatticeWriterOutputIterator::doFlush() {
}

#endif
