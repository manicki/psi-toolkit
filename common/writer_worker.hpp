#ifndef WRITER_WORKER_HDR
#define WRITER_WORKER_HDR

#include <iostream>

#include "lattice_worker.hpp"

class WriterWorker : public LatticeWorker {

public:
    WriterWorker(std::ostream& outputStream, Lattice& lattice)
        :LatticeWorker(lattice), outputStream_(outputStream), currentPos_(0){
    }

    virtual ~WriterWorker() {
    }

protected:
    /**
     * Prints output to outputStream_ and updates cursor position.
     * Returns the new cursor position.
     */
    unsigned int alignOutput_(std::string output) {
        outputStream_ << output;
        currentPos_ += utf8::distance(output.begin(), output.end());
        return currentPos_;
    }

    /**
     * Prints output to outputStream_ and aligns cursor to the required position (pos) if possible.
     * Strings are aligned left.
     * Returns the real new cursor position.
     */
    unsigned int alignOutput_(std::string output, unsigned int pos, char padChar = ' ') {
        outputStream_ << output;
        currentPos_ += utf8::distance(output.begin(), output.end());
        for (; currentPos_ < pos; ++currentPos_) {
            outputStream_ << padChar;
        }
        return currentPos_;
    }

    /**
     * Prints newline to outputStream_ and resets cursor position.
     * Returns the new cursor position (zero).
     */
    unsigned int alignOutputNewline_() {
        outputStream_ << std::endl;
        currentPos_ = 0;
        return currentPos_;
    }

    std::ostream& getOutputStream() { return outputStream_; }

private:
    std::ostream& outputStream_;

    unsigned int currentPos_;

};

#endif
