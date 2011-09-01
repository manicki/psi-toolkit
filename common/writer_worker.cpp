#include "writer_worker.hpp"


WriterWorker::WriterWorker(std::ostream& outputStream, Lattice& lattice):
    LatticeWorker(lattice), outputStream_(outputStream), currentPos_(0) {
}

WriterWorker::~WriterWorker() {
}


int WriterWorker::alignOutput_(std::string output, int pos) {
    outputStream_ << output;
    currentPos_ += utf8::distance(output.begin(), output.end());
    for (; currentPos_ < pos; ++currentPos_) {
        outputStream_ << " ";
    }
    return currentPos_;
}
