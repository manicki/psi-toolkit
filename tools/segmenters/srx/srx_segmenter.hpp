#ifndef SRX_SEGMENTER_HDR
#define SRX_SEGMENTER_HDR

#include <boost/filesystem.hpp>
#include <boost/scoped_ptr.hpp>

#include "regexp.hpp"

#include "annotator.hpp"
#include "annotator_factory.hpp"

#include "lang_specific_processor_file_fetcher.hpp"

#include "srx_rules.hpp"

class SrxSentenceCutter;

class SrxSegmenter : public Annotator {

public:
    class Factory : public AnnotatorFactory {
    private:
        virtual Annotator* doCreateAnnotator(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName();

        virtual std::list<std::list<std::string> > doRequiredLayerTags();

        virtual std::list<std::list<std::string> > doOptionalLayerTags();

        virtual std::list<std::string> doProvidedLayerTags();

        static const std::string DEFAULT_RULE_FILE_SPEC;
    };

    SrxSegmenter(const std::string& lang,
                 boost::filesystem::path rules);

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

    friend class RuleProcessor;
    friend class SrxSentenceCutter;

    void processRule_(const SrxRule& srxRule);
    std::string makeRegexp_(const SrxRule& srxRule);
    std::string makeAllParensNonCapturing_(const std::string& pattern);

    std::vector<boost::shared_ptr<PerlRegExp> > nonBreakingRules_;

    struct BreakingRuleInfo {
        size_t nbOfApplicableNonBreakingRules;
        boost::shared_ptr<PerlRegExp> breakingRule;

        BreakingRuleInfo(size_t aNbOfApplicableNonBreakingRules,
                         boost::shared_ptr<PerlRegExp> aBreakingRule)
            : nbOfApplicableNonBreakingRules(aNbOfApplicableNonBreakingRules),
              breakingRule(aBreakingRule) {
        }
    };

    std::vector<BreakingRuleInfo> breakingRules_;

};

#endif
