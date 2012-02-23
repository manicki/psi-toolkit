#ifndef GOBIO_HDR
#define GOBIO_HDR


#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>

#include "annotator.hpp"
#include "language_dependent_annotator_factory.hpp"
#include "lang_specific_processor_file_fetcher.hpp"

#include "agenda.hpp"
#include "agenda_parser.tpl"
#include "number_master.hpp"
#include "semantics_wrapper.hpp"
// #include "simple_cfg_combinator.tpl"
#include "tgbg_combinator.tpl"


class Gobio : public Annotator {

public:

    typedef Lattice::EdgeDescriptor Edge;

    // typedef std::string Atom;
    typedef int Atom;

    // typedef std::string Category;
    typedef av_matrix<int, Atom> Category;

    typedef Lattice::Score Score;

    typedef number_master Master;

    // typedef number_master SemanticsMachine;
    typedef semantics_wrapper SemanticsMachine;

    // typedef boost::shared_ptr<pe_target_info> Equivalent;

    // typedef simple_cfg_combinator<Category, Rule> Combinator;
    typedef tgbg_combinator<
        Atom,
        Score,
        Master,
        SemanticsMachine // ,
        // Equivalent
    > Combinator;

    // typedef simple_cfg_rule<Category> Rule;
    typedef Combinator::rule_type Rule;

    // typedef std::string Variant;
    typedef Combinator::variant_type Variant;

    typedef fifo_agenda<Edge> Agenda;
    // typedef dyna_agenda<Edge, fifo_agenda<Edge> > Agenda;

    // typedef chart<Category, Score, Variant, Rule, simple_marked_edges_index> Chart;
    typedef chart<Category, Score, Variant, Rule, simple_marked_edges_index> Chart;

    typedef agenda_parser<
        Category,
        Score,
        Variant,
        Rule,
        Combinator,
        Agenda,
        simple_marked_edges_index
    > Parser;

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

        static const std::string DEFAULT_RULE_FILE;
    };

    Gobio(boost::shared_ptr<Combinator> combinatorPtr);

    void parse(Lattice &lattice);

private:

    class Worker : public LatticeWorker {
    public:
        Worker(Gobio& processor, Lattice& lattice);
    private:
        virtual void doRun();
        Gobio& processor_;
    };

    virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice);

    virtual std::string doInfo();

    boost::shared_ptr<Combinator> combinator_;

};


#endif
