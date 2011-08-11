#include "lattice_writer.hpp"

void LatticeWriter::writeLattice(Lattice& lattice, std::ostream& outputStream) {
    boost::scoped_ptr<WriterWorker> worker(doCreateWriterWorker(outputStream, lattice));
    worker->run();
}

LatticeWriter::~LatticeWriter() {
}
