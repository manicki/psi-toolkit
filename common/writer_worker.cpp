#include "writer_worker.hpp"


WriterWorker::WriterWorker(std::ostream& outputStream, Lattice& lattice):
    LatticeWorker(lattice), outputStream_(outputStream) {
}

WriterWorker::~WriterWorker() {
}

