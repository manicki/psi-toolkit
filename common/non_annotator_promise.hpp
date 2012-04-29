#ifndef NON_ANNOTATOR_PROMISE_HDR
#define NON_ANNOTATOR_PROMISE_HDR

#include "processor_promise.hpp"
#include "processor_factory.hpp"

class NonAnnotatorPromise: public ProcessorPromise {
public:
    NonAnnotatorPromise(
        ProcessorFactory* factory,
        const boost::program_options::variables_map& options)
        : factory_(factory), options_(options) {
    }

    virtual ~NonAnnotatorPromise() {
    }

private:
    Processor* doCreateProcessor();

    std::list<std::list<std::string> > doRequiredLayerTags();

    std::list<std::list<std::string> > doOptionalLayerTags();

    std::list<std::string> doProvidedLayerTags();

    AnnotatorFactory::LanguagesHandling doLanguagesHandling() const;

    std::list<std::string> doLanguagesHandled() const;

    bool doIsAnnotator() const;

    virtual double doGetQualityScore() const;

    virtual double doGetEstimatedTime() const;

    virtual std::string doGetName() const;

    virtual boost::shared_ptr<ProcessorPromise> doCloneWithLanguageSet(
        const std::string& langCode) const;

    ProcessorFactory* factory_;
    boost::program_options::variables_map options_;
};

#endif
