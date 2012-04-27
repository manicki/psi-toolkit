#ifndef ANNOTATOR_PROMISE_HDR
#define ANNOTATOR_PROMISE_HDR

#include "processor_promise.hpp"

class AnnotatorPromise: public ProcessorPromise {
public:
    virtual ~AnnotatorPromise() {
    }

    AnnotatorPromise(
        AnnotatorFactory* annotatorFactory,
        const boost::program_options::variables_map& options);

private:
    Processor* doCreateProcessor();

    std::list<std::list<std::string> > doRequiredLayerTags();

    std::list<std::list<std::string> > doOptionalLayerTags();

    std::list<std::string> doProvidedLayerTags();

    AnnotatorFactory::LanguagesHandling doLanguagesHandling() const;

    std::list<std::string> doLanguagesHandled() const;

    virtual bool doIsAnnotator() const;
    virtual bool doIsReader() const;
    virtual bool doIsWriter() const;

    AnnotatorFactory* annotatorFactory_;
    boost::program_options::variables_map options_;
};

#endif
