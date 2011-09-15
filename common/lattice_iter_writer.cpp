#include "lattice_iter_writer.hpp"

void LatticeIterWriter::writeLattice(
    Lattice& lattice,
    LatticeWriterOutputIterator& outputIterator
) {
    //TODO
}

LatticeIterWriter::~LatticeIterWriter() {
}

std::string LatticeIterWriter::doInfo() {
    return "Lattice iter writer";
}