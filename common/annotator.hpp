#ifndef ANNOTATOR_HDR
#define ANNOTATOR_HDR

#include <boost/program_options/options_description.hpp>
#include <boost/scoped_ptr.hpp>

#include "lattice.hpp"
#include "layer_tag_collection.hpp"

#include "processor.hpp"

#include "lattice_worker.hpp"

/*!
  Components capable of adding annotations (edges) to a PSI lattice
  are (tokenisers, lemmatisers, parsers etc.) are called *annotators*.
*/
class Annotator : public Processor {

public:

    /**
     * Adds new annotations to `lattice`. Returns false if the annotator
     * was unable to proceed.
     *
     * `annotate` may return true and add no annotations, e.g. a named entity
     * recogniser could return true if no named entities were found.
     */
    bool annotate(Lattice&);

private:
    virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice) = 0;

};

#endif
