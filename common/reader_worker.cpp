#include "reader_worker.hpp"

ReaderWorker::ReaderWorker(std::istream& inputStream, Lattice& lattice):
    LatticeWorker(lattice), inputStream_(inputStream) {
}

