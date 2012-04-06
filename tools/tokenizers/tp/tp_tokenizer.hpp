#ifndef TP_TOKENIZER_HDR
#define TP_TOKENIZER_HDR

#include <boost/filesystem.hpp>

#include "annotator.hpp"
#include "language_dependent_annotator_factory.hpp"
#include "tp_basic_tokenizer_rule_set.hpp"

#include "lang_specific_processor_file_fetcher.hpp"

class TpTokenizer : public Annotator {

public:
    class Factory : public LanguageDependentAnnotatorFactory {
    private:
        virtual Annotator* doCreateAnnotator(
            const boost::program_options::variables_map& options);

        virtual void doAddLanguageIndependentOptionsHandled(
            boost::program_options::options_description& optionsDescription);

        virtual std::string doGetName();
        virtual std::list<std::string> doGetAliases();
        virtual boost::filesystem::path doGetFile() const;

        virtual std::list<std::list<std::string> > doRequiredLayerTags();

        virtual std::list<std::list<std::string> > doOptionalLayerTags();

        virtual std::list<std::string> doProvidedLayerTags();

        virtual std::string doGetContinuation(
            const boost::program_options::variables_map& options) const;

        std::map<std::string, std::string> parseMappingOption_(
            const std::string& mappingOption,
            const std::string& lang);

        std::map<std::string, boost::filesystem::path> substituteMapping_(
            const std::map<std::string, std::string>& rawMapping,
            const LangSpecificProcessorFileFetcher& fileFetcher);

        static const std::string DEFAULT_RULE_FILE_SPEC;
        static const std::string DEFAULT_RULE_FILE_MAPPING;

        static const size_t DEFAULT_HARD_LIMIT;
        static const size_t DEFAULT_SOFT_LIMIT;
    };

    TpTokenizer(
        boost::filesystem::path rules,
        const std::map<std::string, boost::filesystem::path>& mapping,
        size_t hardLimit,
        size_t softLimit);

private:

    class Worker : public LatticeWorker {
    public:
        Worker(Processor& processor, Lattice& lattice);
    private:
        virtual void doRun();
        Processor& processor_;
    };

    virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice);

    virtual std::string doInfo();

    boost::scoped_ptr<TPBasicTokenizerRuleSet> ruleSet_;

    size_t hardLimit_;
    size_t softLimit_;
};

#endif
