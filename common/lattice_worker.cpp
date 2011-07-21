#include "lattice_worker.hpp"

LatticeWorker::LatticeWorker(Lattice& lattice):lattice_(lattice) {
}

void LatticeWorker::run() {
    doRun();
}
