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

    LatticeIterWriter(
        Lattice& lattice,
        LatticeWriterOutputIterator& outputIterator,
        bool linear,
        std::string basicTag,
        std::vector<std::string> handledTags
    );

    virtual ~LatticeIterWriter();

private:

    Lattice & lattice_;
    LatticeWriterOutputIterator & outputIterator_;
    bool linear_;
    std::string basicTag_;
    std::vector<std::string> handledTags_;

    virtual std::string doInfo();

    void doRun();

    bool isHandledTag_(std::string tagName);

};

#endif
