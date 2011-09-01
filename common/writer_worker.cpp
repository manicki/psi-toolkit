#include "writer_worker.hpp"


WriterWorker::WriterWorker(std::ostream& outputStream, Lattice& lattice):
    LatticeWorker(lattice), outputStream_(outputStream), currentPos_(0) {
}

WriterWorker::~WriterWorker() {
}

unsigned int WriterWorker::alignOutput_(std::string output, unsigned int pos, char padChar) {
    outputStream_ << output;
    currentPos_ += utf8::distance(output.begin(), output.end());
    for (; currentPos_ < pos; ++currentPos_) {
        outputStream_ << padChar;
    }
    return currentPos_;
}

unsigned int WriterWorker::alignOutput_(unsigned int output, unsigned int pos, char padChar) {
    unsigned int number = output;
    int numDigits = 0;
    while (number > 0) {
        ++numDigits;
        number /= 10;
    }
    if (output == 0) {
        numDigits = 1;
    }
    for (; currentPos_ < pos - numDigits; ++currentPos_) {
        outputStream_ << padChar;
    }
    outputStream_ << output;
    currentPos_ += numDigits;
    return currentPos_;
}
