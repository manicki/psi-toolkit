#ifndef TP_TOKENIZER_HDR
#define TP_TOKENIZER_HDR

#include "annotator.hpp"
#include "annotator_factory.hpp"
#include "tp_basic_tokenizer_rule_set.hpp"

class TpTokenizer : public Annotator {

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

    };

    TpTokenizer(const std::string& lang);

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
};

#endif
