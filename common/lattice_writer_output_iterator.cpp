#include "lattice_writer_output_iterator.hpp"

void LatticeWriterOutputIterator::putElement(const std::string& element) {
    doPutElement(element);
}

void LatticeWriterOutputIterator::openAlternative() {
    doOpenAlternative();
}

void LatticeWriterOutputIterator::closeAlternative() {
    doCloseAlternative();
}

void LatticeWriterOutputIterator::openGroup(const std::string& group) {
    doOpenGroup(group);
}

void LatticeWriterOutputIterator::closeGroup(const std::string& group) {
    doCloseGroup(group);
}

void LatticeWriterOutputIterator::flush() {
    doFlush();
}

LatticeWriterOutputIterator::~LatticeWriterOutputIterator() {
}
