#ifndef READER_WORKER_HDR
#define READER_WORKER_HDR

#include <iostream>

#include "lattice_worker.hpp"

class ReaderWorker : public LatticeWorker {

public:
    ReaderWorker(std::istream& inputStream, Lattice& lattice);

    virtual ~ReaderWorker();

protected:
    std::istream& inputStream_;

};

#endif
