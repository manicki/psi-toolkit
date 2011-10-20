#ifndef WRITER_WORKER_HDR
#define WRITER_WORKER_HDR

#include <iostream>

#include "lattice_worker.hpp"

class WriterWorker : public LatticeWorker {

public:
    WriterWorker(std::ostream& outputStream, Lattice& lattice)
        :LatticeWorker(lattice), outputStream_(outputStream) {
    }

    virtual ~WriterWorker() {
    }

    std::ostream& getOutputStream() {
        return outputStream_;
    }

private:
    std::ostream& outputStream_;
};

#endif
