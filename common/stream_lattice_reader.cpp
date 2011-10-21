#include "stream_lattice_reader.hpp"

StreamLatticeReader::StreamLatticeReader() {
}

void StreamLatticeReader::readIntoLattice(const std::string& fileName, Lattice& lattice) {
    std::ifstream inputStream(fileName.c_str());
    LatticeReader::readIntoLattice(inputStream, lattice);
}
