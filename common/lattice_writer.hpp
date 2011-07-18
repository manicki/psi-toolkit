#ifndef LATTICE_WRITER_HDR
#define LATTICE_WRITER_HDR

#include <string>
#include <ostream>

#include "lattice.hpp"

class LatticeWriter {

public:
    virtual void writeLattice(std::ostream& outputStream, Lattice& lattice) = 0;

    /**
     * Gets format name (eg. "UTT", "BIN" etc.)
     */
    virtual std::string getFormatName() = 0;
};

#endif
