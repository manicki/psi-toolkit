#ifndef WRITER_WORKER_HDR
#define WRITER_WORKER_HDR

#include <iostream>

#include "lattice_worker.hpp"

class WriterWorker : public LatticeWorker {

public:
    WriterWorker(std::ostream& outputStream, Lattice& lattice);

    virtual ~WriterWorker();

protected:
    std::ostream& outputStream_;

    unsigned int currentPos_;

    /**
     * Prints output to outputStream_ and aligns cursor to the required position (pos) if possible.
     * Strings are aligned left.
     * Returns the real new cursor position.
     */
    unsigned int alignOutput_(std::string output, unsigned int pos, char padChar = ' ');

    /**
     * Prints output to outputStream_ and aligns cursor to the required position (pos) if possible.
     * Numbers are aligned right.
     * Returns the real new cursor position.
     */
    unsigned int alignOutput_(unsigned int output, unsigned int pos, char padChar = ' ');

};

#endif
