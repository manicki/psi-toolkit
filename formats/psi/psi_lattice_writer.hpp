#ifndef PSI_LATTICE_WRITER_HDR
#define PSI_LATTICE_WRITER_HDR

class PsiLatticeWriter {

public:
    virtual void writeLattice(std::ostream& outputStream, Lattice& lattice) = 0;

};


#endif
