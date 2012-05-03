#ifndef PROCESSOR_PROMISE_HDR
#define PROCESSOR_PROMISE_HDR

#include "annotator_factory.hpp"
#include "lattice_reader_factory.hpp"
#include "lattice_writer_factory.hpp"

class ProcessorPromise {
public:
    virtual ~ProcessorPromise() {
    }

    ProcessorPromise(
        ProcessorFactory* factory,
        const boost::program_options::variables_map& options)
        : factory_(factory), options_(options) {
    }

    boost::shared_ptr<Processor> createProcessor();

    std::list<std::list<std::string> > requiredLayerTags();

    std::list<std::list<std::string> > optionalLayerTags();

    std::list<std::string> providedLayerTags();

    AnnotatorFactory::LanguagesHandling languagesHandling() const;

    std::list<std::string> languagesHandled() const;

    bool isAnnotator() const;

    template<class Source>
    bool isReader() const {
        return dynamic_cast<const LatticeReaderFactory<Source>* >(factory_) != 0;
    }

    template<class Sink>
    bool isWriter() const {
        return dynamic_cast<const LatticeWriterFactory<Sink>* >(factory_) != 0;
    }

    double getQualityScore() const;

    double getEstimatedTime() const;

    std::string getName() const;

    boost::shared_ptr<ProcessorPromise> cloneWithLanguageSet(
        const std::string& langCode);

    bool checkRequirements(std::ostream & message) const;

    std::string getContinuation() const;

private:
    virtual Processor* doCreateProcessor() = 0;

    virtual std::list<std::list<std::string> > doRequiredLayerTags() = 0;

    virtual std::list<std::list<std::string> > doOptionalLayerTags() = 0;

    virtual std::list<std::string> doProvidedLayerTags() = 0;

    virtual AnnotatorFactory::LanguagesHandling doLanguagesHandling() const = 0;

    virtual std::list<std::string> doLanguagesHandled() const = 0;

    virtual bool doIsAnnotator() const = 0;

    virtual double doGetQualityScore() const = 0;

    virtual double doGetEstimatedTime() const = 0;

    virtual std::string doGetName() const = 0;

    virtual boost::shared_ptr<ProcessorPromise> doCloneWithLanguageSet(
        const std::string& langCode) = 0;

protected:
    ProcessorFactory* factory_;
    boost::program_options::variables_map options_;
    boost::shared_ptr<Processor> processor_;
};

#endif
