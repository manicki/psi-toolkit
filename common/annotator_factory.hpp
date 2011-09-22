#ifndef ANNOTATOR_FACTORY_HDR
#define ANNOTATOR_FACTORY_HDR

#include <boost/program_options/options_description.hpp>

#include "processor_factory.hpp"
#include "annotator.hpp"

#include <string>
#include <list>

class AnnotatorFactory : public ProcessorFactory {

public:
    Annotator* createAnnotator(const boost::program_options::variables_map& options);

    /**
     * Returns layer tags that must be provided in the lattice for
     * the annotator to proceed.
     *
     * This information can be used by the framework to automagically
     * run other annotators before the given annotator is launched.
     */
    std::list<std::list<std::string> > requiredLayerTags();

    /**
     * Returns layer tags that might be useful for the annotator.
     *
     * This information can be used by the framework to run other
     * (non-obligatory) annotators before the given annotator is launched.
     */
    std::list<std::list<std::string> > optionalLayerTags();

    /**
     * Layer tags provided by the given annotator. This information
     * could be used by the framework when considering `requiredLayerTags`
     * and `optionalLayerTags` of another annotator.
     */
    std::list<std::string> providedLayerTags();;

private:
    virtual Annotator* doCreateProcessor(const boost::program_options::variables_map& options);

    virtual Annotator* doCreateAnnotator(const boost::program_options::variables_map& options) = 0;


    virtual std::list<std::list<std::string> > doRequiredLayerTags() = 0;

    virtual std::list<std::list<std::string> > doOptionalLayerTags() = 0;

    virtual std::list<std::string> doProvidedLayerTags() = 0;

    virtual std::string doGetContinuation(
        const boost::program_options::variables_map& options) const;

};

#endif
