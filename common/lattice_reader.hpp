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

    class CannotOpenFileException : public std::runtime_error {
    public:
        CannotOpenFileException() : std::runtime_error("cannot open a file") { }
    };

    /**
     * Reads from `inputStream` into `lattice`. If lattice is not empty,
     * new edges will be appended at the end of lattice.
     */
    void readIntoLattice(std::istream& inputStream, Lattice& lattice);

    /**
     * Reads from file `fileName` into `lattice`. If lattice is not empty,
     * new edges will be appended at the end of lattice.
     */
    void readIntoLattice(const std::string& fileName, Lattice& lattice);

    /**
     * Gets format name (eg. "UTT", "BIN" etc.)
     */
    virtual std::string getFormatName() { }

private:
    virtual ReaderWorker* doCreateReaderWorker(std::istream& inputStream, Lattice& lattice) = 0;

};


#endif
