#include "lattice_reader.hpp"

void LatticeReader::readIntoLattice(std::istream& inputStream, Lattice& lattice) {
    doReadIntoLattice(inputStream, lattice);
}

void LatticeReader::readIntoLattice(const std::string& fileName, Lattice& lattice) {
    std::ifstream inputStream(fileName.c_str());
    readIntoLattice(inputStream, lattice);
}


