#ifndef LATTICE_READER_HDR
#define LATTICE_READER_HDR

#include <iostream>
#include <string>

#include "lattice.hpp"


class LatticeReader {

public:
    /**
     * Reads from `inputStream` into `lattice`. If lattice is not empty,
     * new edges will be appended at the end of lattice.
     */
    virtual void readIntoLattice(std::istream& inputStream, Lattice& lattice);

    /**
     * Gets format name (eg. "UTT", "BIN" etc.)
     */
    virtual std::string getFormatName();
};


#endif
