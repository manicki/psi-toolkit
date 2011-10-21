#ifndef READER_WORKER_HDR
#define READER_WORKER_HDR

#include <iostream>

#include "lattice_worker.hpp"

template<typename Source>
class ReaderWorker : public LatticeWorker {

public:
    ReaderWorker(Source& source, Lattice& lattice)
        :LatticeWorker(lattice), inputStream_(source) {
    }

    virtual ~ReaderWorker() {
    }

protected:
    std::istream& inputStream_;

};

#endif
