#ifndef UTT_READER_HDR
#define UTT_READER_HDR

#include "lattice_reader.hpp"


class UTTReader : public LatticeReader {

public:
    /**
     * Reads from `inputStream` into `lattice`. If lattice is not empty,
     * new edges will be appended at the end of lattice.
     */
    void readIntoLattice(std::istream& inputStream, Lattice& lattice);

    /**
     * Gets format name (eg. "UTT", "BIN" etc.)
     */
    std::string getFormatName();
};


#endif
