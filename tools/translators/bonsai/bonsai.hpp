#ifndef BONSAI_HDR
#define BONSAI_HDR

#include "config.hpp"
#include "annotator.hpp"
#include "language_dependent_annotator_factory.hpp"
#include "lang_specific_processor_file_fetcher.hpp"
#include <boost/program_options.hpp>
#include <fstream>

#include <string>
#include <vector>
#include <map>
#include <sstream>

#include "TransferTypes.hpp"
#include "Translator.hpp"

class Bonsai : public Annotator {
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

        Bonsai(poleng::bonsai::TranslatorPtr translator, const std::string& langCode);
        void translate(Lattice &lattice);

    private:
        class Worker : public LatticeWorker {
            public:
                Worker(Bonsai& processor, Lattice& lattice);
            private:
                virtual void doRun();
                Bonsai& processor_;
        };
        virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice);
        virtual std::string doInfo();

        poleng::bonsai::TranslatorPtr translator_;
        std::string langCode_;
};

#endif
