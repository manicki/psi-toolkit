#ifndef ANNOTATOR_FACTORY_HDR
#define ANNOTATOR_FACTORY_HDR

#include "processor_factory.hpp"
#include "annotator.hpp"

class AnnotatorFactory : public ProcessorFactory {
    
public:
    Annotator* createAnnotator(boost::program_options::variables_map options);

    /**
     * Returns layer tags that must be provided in the lattice for
     * the annotator to proceed.
     *
     * This information can be used by the framework to automagically
     * run other annotators before the given annotator is launched.
     */
    std::list<LayerTagCollection> requiredLayerTags();

    /**
     * Returns layer tags that might be useful for the annotator.
     *
     * This information can be used by the framework to run other
     * (non-obligatory) annotators before the given annotator is launched.
     */
    std::list<LayerTagCollection> optionalLayerTags();

    /**
     * Layer tags provided by the given annotator. This information
     * could be used by the framework when considering `requiredLayerTags`
     * and `optionalLayerTags` of another annotator.
     */
    LayerTagCollection providedLayerTags();;

private:
    virtual Annotator* doCreateProcessor(boost::program_options::variables_map options);

    virtual Annotator* doCreateAnnotator(boost::program_options::variables_map options) = 0;


    virtual std::list<LayerTagCollection> doRequiredLayerTags() = 0;

    virtual std::list<LayerTagCollection> doOptionalLayerTags() = 0;

    virtual LayerTagCollection doProvidedLayerTags() = 0;

};

#endif
