#ifndef UTT_LATTICE_READER_HDR
#define UTT_LATTICE_READER_HDR


#include "lattice_reader.hpp"


class PsiLatticeReader : public LatticeReader {

public:

    /**
     * Gets format name (here: "UTT").
     */
    std::string getFormatName();

private:
    virtual std::string doInfo();

    virtual void doReadIntoLattice(std::istream& inputStream, Lattice& lattice);

};


#endif
