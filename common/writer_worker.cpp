#include "writer_worker.hpp"


WriterWorker::WriterWorker(std::ostream& outputStream, Lattice& lattice):
    LatticeWorker(lattice), outputStream_(outputStream), currentPos_(0) {
}

WriterWorker::~WriterWorker() {
}

unsigned int WriterWorker::alignOutput_(std::string output) {
    outputStream_ << output;
    currentPos_ += utf8::distance(output.begin(), output.end());
    return currentPos_;
}

unsigned int WriterWorker::alignOutput_(std::string output, unsigned int pos, char padChar) {
    outputStream_ << output;
    currentPos_ += utf8::distance(output.begin(), output.end());
    for (; currentPos_ < pos; ++currentPos_) {
        outputStream_ << padChar;
    }
    return currentPos_;
}

unsigned int WriterWorker::alignOutputNewline_() {
    outputStream_ << std::endl;
    currentPos_ = 0;
    return currentPos_;
}
