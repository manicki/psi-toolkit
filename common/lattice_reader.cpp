#include "lattice_reader.hpp"

#include <boost/scoped_ptr.hpp>

void LatticeReader::readIntoLattice(std::istream& inputStream, Lattice& lattice) {
    boost::scoped_ptr<ReaderWorker> worker(doCreateReaderWorker(inputStream, lattice));
    worker->run();
}

void LatticeReader::readIntoLattice(const std::string& fileName, Lattice& lattice) {
    std::ifstream inputStream(fileName.c_str());
    readIntoLattice(inputStream, lattice);
}

LatticeReader::~LatticeReader() {
}

