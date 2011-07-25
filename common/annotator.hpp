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

#ifdef BLADSDSD
    /**
     * Returns description of the options handled by the annotator.
     */
    virtual boost::options_description options() = 0;

    /**
     * Returns layer tags that must be provided in the lattice for
     * the annotator to proceed.
     *
     * This information can be used by the framework to automagically
     * run other annotators before the given annotator is launched.
     */
    virtual std::list<LayerTagCollection> requiredLayerTags() = 0;

    /**
     * Returns layer tags that might be useful for the annotator.
     *
     * This information can be used by the framework to run other
     * (non-obligatory) annotators before the given annotator is launched.
     */
    virtual std::list<LayerTagCollection> optionalLayerTags() = 0;

    /**
     * Layer tags provided by the given annotator. This information
     * could be used by the framework when considering `requiredLayerTags`
     * and `optionalLayerTags` of another annotator.
     */
    virtual LayerTagCollection providedLayerTags() = 0;
#endif

private:
    virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice) = 0;

};

#endif
