#ifndef LATTICE_READER_HDR
#define LATTICE_READER_HDR

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include <boost/scoped_ptr.hpp>

#include "lattice.hpp"
#include "reader_worker.hpp"
#include "processor.hpp"

template<typename Source>
class LatticeReader : public Processor {

public:

    /**
     * Reads from `source` into `lattice`. If lattice is not empty,
     * new edges will be appended at the end of lattice.
     */
    void readIntoLattice(Source& source, Lattice& lattice) {
        boost::scoped_ptr<ReaderWorker<Source> > worker(doCreateReaderWorker(source, lattice));
        worker->run();
    }

    /**
     * Gets format name (eg. "UTT", "BIN" etc.)
     */
    virtual std::string getFormatName() = 0;

    virtual ~LatticeReader() {
    }

private:
    virtual ReaderWorker<Source>* doCreateReaderWorker(Source& source, Lattice& lattice) = 0;

};


#endif
