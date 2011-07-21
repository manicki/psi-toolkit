#ifndef TXT_LATTICE_READER_HDR
#define TXT_LATTICE_READER_HDR


#include "lattice_reader.hpp"


class TxtLatticeReader : public LatticeReader {

public:
    virtual std::string getFormatName();

private:
    virtual std::string doInfo();

    virtual void doReadIntoLattice(std::istream& inputStream, Lattice& lattice);

    void appendParagraphToLattice_(std::string paragraph, Lattice& lattice);

};

#endif
