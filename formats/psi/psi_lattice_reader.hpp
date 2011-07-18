#ifndef PSI_LATTICE_READER_HDR
#define PSI_LATTICE_READER_HDR


#include "lattice_reader.hpp"


class PsiLatticeReader : public LatticeReader {

public:
    /**
     * Reads from `inputStream` into `lattice`. If lattice is not empty,
     * new edges will be appended at the end of lattice.
     */
    void readIntoLattice(std::istream& inputStream, Lattice& lattice);

    /**
     * Gets format name (here: "Psi").
     */
    std::string getFormatName();

};


#endif
