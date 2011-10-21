#ifndef LATTICE_READER_HDR
#define LATTICE_READER_HDR

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "lattice.hpp"
#include "reader_worker.hpp"
#include "processor.hpp"

class LatticeReader : public Processor {

public:

    /**
     * Reads from `inputStream` into `lattice`. If lattice is not empty,
     * new edges will be appended at the end of lattice.
     */
    void readIntoLattice(std::istream& inputStream, Lattice& lattice) {
        boost::scoped_ptr<ReaderWorker> worker(doCreateReaderWorker(inputStream, lattice));
        worker->run();
    }

    /**
     * Gets format name (eg. "UTT", "BIN" etc.)
     */
    virtual std::string getFormatName() = 0;

    virtual ~LatticeReader() {
    }

private:
    virtual ReaderWorker* doCreateReaderWorker(std::istream& inputStream, Lattice& lattice) = 0;

};


#endif
