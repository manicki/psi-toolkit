#ifndef WRITER_WORKER_HDR
#define WRITER_WORKER_HDR

#include <iostream>

#include "lattice_worker.hpp"

template<typename Sink>
class WriterWorker : public LatticeWorker {

public:
    WriterWorker(Sink& sink, Lattice& lattice)
        :LatticeWorker(lattice), sink_(sink) {
    }

    virtual ~WriterWorker() {
    }

    Sink& getOutputStream() {
        return sink_;
    }

private:
    std::ostream& sink_;
};

#endif
