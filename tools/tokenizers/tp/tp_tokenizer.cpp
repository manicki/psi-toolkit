#include "tp_tokenizer.hpp"

#include "logging.hpp"

TpTokenizer::Worker::Worker(Processor& processor, Lattice& lattice):
    LatticeWorker(lattice), processor_(processor) {
}

void TpTokenizer::Worker::doRun() {
    DEBUG("NA RAZIE NIC NIE ROBIĘ");
}
