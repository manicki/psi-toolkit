#ifndef WRITER_WORKER_HDR
#define WRITER_WORKER_HDR

#include <iostream>

#include "lattice_worker.hpp"

class WriterWorker : public LatticeWorker {

public:
    WriterWorker(std::ostream& outputStream, Lattice& lattice);

protected:
    std::ostream& outputStream_;

};

#endif
