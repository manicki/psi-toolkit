#ifndef PSI_LATTICE_WRITER_HDR
#define PSI_LATTICE_WRITER_HDR

#include "lattice_writer.hpp"

class PsiLatticeWriter : public LatticeWriter {

public:
    void writeLattice(std::ostream& outputStream, Lattice& lattice);

};


#endif
