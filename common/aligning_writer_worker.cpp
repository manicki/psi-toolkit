#include "aligning_writer_worker.hpp"

AligningWriterWorker::AligningWriterWorker(std::ostream& outputStream, Lattice& lattice)
    :WriterWorker(outputStream, lattice), currentPos_(0) {
}

AligningWriterWorker::~AligningWriterWorker() {
}

unsigned int AligningWriterWorker::alignOutput_(std::string output) {
    getOutputStream() << output;
    currentPos_ += utf8::distance(output.begin(), output.end());
    return currentPos_;
}

unsigned int AligningWriterWorker::alignOutput_(std::string output, unsigned int pos, char padChar) {
    getOutputStream() << output;
    currentPos_ += utf8::distance(output.begin(), output.end());
    for (; currentPos_ < pos; ++currentPos_) {
        getOutputStream() << padChar;
    }
    return currentPos_;
}

unsigned int AligningWriterWorker::alignOutputNewline_() {
    getOutputStream() << std::endl;
    currentPos_ = 0;
    return currentPos_;
}
