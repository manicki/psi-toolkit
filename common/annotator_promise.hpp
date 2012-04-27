#ifndef ANNOTATOR_PROMISE_HDR
#define ANNOTATOR_PROMISE_HDR

#include "processor_promise.hpp"

class AnnotatorPromise: public ProcessorPromise {
public:
    virtual ~AnnotatorPromise() {
    }

    AnnotatorPromise(
        AnnotatorFactory& annotatorFactory,
        const boost::program_options::variables_map& options);

private:
    Processor* doCreateProcessor();

    std::list<std::list<std::string> > doRequiredLayerTags();

    std::list<std::list<std::string> > doOptionalLayerTags();

    std::list<std::string> doProvidedLayerTags();

    AnnotatorFactory::LanguagesHandling doLanguagesHandling() const;

    std::list<std::string> doLanguagesHandled() const;

    bool doIsAnnotator() const = 0;
    bool doIsReader() const = 0;
    bool doIsWriter() const = 0;

    AnnotatorFactory& annotatorFactory_;
    boost::program_options::variables_map options_;
};

#endif
