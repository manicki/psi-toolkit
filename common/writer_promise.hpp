#ifndef WRITER_PROMISE_HDR
#define WRITER_PROMISE_HDR

#include "processor_promise.hpp"
#include "lattice_writer_factory.hpp"

class WriterPromise: public ProcessorPromise {
public:
    WriterPromise(
        ProcessorFactory* writerFactory,
        const boost::program_options::variables_map& options)
        :writerFactory_(writerFactory), options_(options) {
    }

    virtual ~WriterPromise() {
    }

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

    ProcessorFactory* writerFactory_;
    boost::program_options::variables_map options_;
};

#endif
