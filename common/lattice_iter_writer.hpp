#ifndef LATTICE_ITER_WRITER_HDR
#define LATTICE_ITER_WRITER_HDR

#include <string>

#include "lattice.hpp"
#include "lattice_writer_output_iterator.hpp"
#include "processor.hpp"

/*!
  Lattice iter writer takes a lattice and writes its content to the lattice output iterator.
*/
class LatticeIterWriter : public Processor {

public:
    void writeLattice(Lattice& lattice, LatticeWriterOutputIterator& outputIterator);

    virtual ~LatticeIterWriter();

private:
    virtual std::string doInfo();

};

#endif
