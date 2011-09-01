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

    int currentPos_;

    /**
     * Prints output to outputStream_ and aligns cursor to the required position (pos) if possible.
     * Returns the real new cursor position.
     */
    int alignOutput_(std::string output, int pos);

};

#endif
