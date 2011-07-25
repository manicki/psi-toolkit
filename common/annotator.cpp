#include "annotator.hpp"

bool Annotator::annotate(Lattice& lattice) {
    boost::scoped_ptr<LatticeWorker> worker(doCreateLatticeWorker(lattice));
    worker->run();

    return true;
}
