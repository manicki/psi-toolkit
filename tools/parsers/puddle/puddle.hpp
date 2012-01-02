#ifndef PUDDLE_HDR
#define PUDDLE_HDR

#include "config.hpp"
#include "annotator.hpp"
#include "language_dependent_annotator_factory.hpp"
#include "lang_specific_processor_file_fetcher.hpp"

#include "tagset.hpp"
#include "rule_loader.hpp"
#include "rule_matcher.hpp"
#include "rule.hpp"
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include "rule_modifier.hpp"
#include "lattice_wrapper.hpp"
#include "tagset_loader.hpp"


namespace poleng
{

namespace bonsai
{

namespace puddle
{

class Puddle : public Annotator {
    public:
        class Factory : public LanguageDependentAnnotatorFactory {
            virtual Annotator* doCreateAnnotator(
                    const boost::program_options::variables_map& options);

            virtual void doAddLanguageIndependentOptionsHandled(
                boost::program_options::options_description& optionsDescription);

            virtual std::string doGetName();
            virtual boost::filesystem::path doGetFile();

            virtual std::list<std::list<std::string> > doRequiredLayerTags();

            virtual std::list<std::list<std::string> > doOptionalLayerTags();

            virtual std::list<std::string> doProvidedLayerTags();

            static const std::string DEFAULT_TAGSET_FILE;
            static const std::string DEFAULT_RULE_FILE;
        };

        Puddle();
        Puddle(TagsetPtr tagset_, RulesPtr rules_);
        virtual void setTagset(bonsai::puddle::TagsetPtr tagset_);
        //@todo: po kiego te funkcje sa wirtualne?
        virtual void setRules(bonsai::puddle::RulesPtr rules_);
        bool parse(Lattice &lattice);

    protected:
        TagsetPtr tagset;
        RulesPtr rules;
        RuleMatcherPtr ruleMatcher;
        RuleModifierPtr ruleModifier;

        bool syntok;
        bool verbose;
        bool describe;
        bool norepeats;

    private:
        void initProperties();

        class Worker : public LatticeWorker {
            public:
                Worker(Puddle& processor, Lattice& lattice);
            private:
                virtual void doRun();
                Puddle& processor_;
        };

        virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice);

        virtual std::string doInfo();

};

typedef boost::shared_ptr<Puddle> PuddlePtr;

}

}

}

#endif
