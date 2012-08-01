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

    virtual double doGetQualityScore() const;

    virtual double doGetEstimatedTime() const;

    virtual std::string doGetName() const;

    virtual boost::shared_ptr<ProcessorPromise> doCloneWithLanguageSet(
        const std::string& langCode);
};

#endif
