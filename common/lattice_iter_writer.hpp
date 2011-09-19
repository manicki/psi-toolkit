#ifndef LATTICE_ITER_WRITER_HDR
#define LATTICE_ITER_WRITER_HDR

#include <queue>
#include <string>

#include <boost/algorithm/string/trim.hpp>
#include <boost/foreach.hpp>

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
        bool noAlts,
        bool noBlank,
        std::string basicTag,
        std::vector<std::string> handledTags
    ) :
        lattice_(lattice),
        outputIterator_(outputIterator),
        linear_(linear),
        noAlts_(noAlts),
        noBlank_(noBlank),
        basicTag_(basicTag),
        handledTags_(handledTags)
    { }

    virtual ~LatticeIterWriter() { }

    void run();

private:

    Lattice & lattice_;
    LatticeWriterOutputIterator & outputIterator_;
    bool linear_;
    bool noAlts_;
    bool noBlank_;
    std::string basicTag_;
    std::vector<std::string> handledTags_;

    virtual std::string doInfo();

    void doRun();

    bool isHandledTag_(std::string tagName);

};

#endif
