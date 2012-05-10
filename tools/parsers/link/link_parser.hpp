#ifndef LINK_PARSER_HDR
#define LINK_PARSER_HDR


#include <boost/program_options.hpp>

#include "annotator.hpp"
#include "language_dependent_annotator_factory.hpp"
#include "lang_specific_processor_file_fetcher.hpp"
#include "plugin/link_parser_adapter_interface.hpp"


class LinkParser : public Annotator {

public:

    class Factory : public LanguageDependentAnnotatorFactory {
        virtual Annotator* doCreateAnnotator(
            const boost::program_options::variables_map& options);

        virtual void doAddLanguageIndependentOptionsHandled(
            boost::program_options::options_description& optionsDescription);

        virtual std::string doGetName() const;
        virtual boost::filesystem::path doGetFile() const;

        virtual std::list<std::list<std::string> > doRequiredLayerTags();

        virtual std::list<std::list<std::string> > doOptionalLayerTags();

        virtual std::list<std::string> doProvidedLayerTags();
    };

    LinkParser(std::string language);

    LinkParser(
        std::string dictionaryName,
        std::string postProcessFileName,
        std::string constituentKnowledgeName,
        std::string affixName
    );

    ~LinkParser();

    LinkParserAdapterInterface * getAdapter();
    bool isActive();

    void parse(Lattice &lattice);

private:

    class Worker : public LatticeWorker {
    public:
        Worker(LinkParser& processor, Lattice& lattice);
    private:
        virtual void doRun();
        LinkParser& processor_;
    };

    virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice);

    virtual std::string doInfo();

    LinkParserAdapterInterface * adapter_;

};


#endif
