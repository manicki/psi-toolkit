#ifndef LATTICE_WORKER_HDR
#define LATTICE_WORKER_HDR

#include "lattice.hpp"

/*!
  Lattice worker is responsible for processing a particular lattice.
*/
class LatticeWorker {

public:
    LatticeWorker(Lattice& lattice);

    void run();

    virtual ~LatticeWorker();

protected:
    Lattice& lattice_;

private:
    virtual void doRun() = 0;
};

#endif
