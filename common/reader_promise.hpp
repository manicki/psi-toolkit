#ifndef READER_PROMISE_HDR
#define READER_PROMISE_HDR

#include "processor_promise.hpp"
#include "lattice_reader_factory.hpp"

class ReaderPromise: public ProcessorPromise {
public:
    template<class Source>
    ReaderPromise(
        LatticeReaderFactory<Source>& readerFactory,
        const boost::program_options::variables_map& options)
        : readerFactory_(readerFactory), options_(options) {
    }

    virtual ~ReaderPromise() {
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

    ProcessorFactory& readerFactory_;
    boost::program_options::variables_map options_;
};

#endif
