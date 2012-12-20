#include "gobio.hpp"


Annotator* Gobio::Factory::doCreateAnnotator(
    const boost::program_options::variables_map & options
) {
    std::string lang = options["lang"].as<std::string>();
    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);

    std::string rulesPathString;

    if (options.count("rules")) {
        std::string rulesFilename = options["rules"].as<std::string>();
        boost::filesystem::path rulesPath = fileFetcher.getOneFile(rulesFilename);
        rulesPathString = rulesPath.string();
    }

    return new Gobio(rulesPathString);
}

void Gobio::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription
) {
    optionsDescription.add_options()
        ("rules",
        boost::program_options::value<std::string>()->default_value(DEFAULT_RULE_FILE),
        "rules file");
}

std::string Gobio::Factory::doGetName() const {
    return "gobio";
}

boost::filesystem::path Gobio::Factory::doGetFile() const {
    return __FILE__;
}

std::list<std::list<std::string> > Gobio::Factory::doRequiredLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::list<std::string> > Gobio::Factory::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> Gobio::Factory::doProvidedLayerTags() {
    std::list<std::string> layerTags;
    layerTags.push_back("gobio");
    layerTags.push_back("parse");
    return layerTags;
}

const std::string Gobio::Factory::DEFAULT_RULE_FILE
    = "%ITSDATA%/%LANG%/rules.g";

LatticeWorker* Gobio::doCreateLatticeWorker(Lattice & lattice) {
    return new Worker(*this, lattice);
}

Gobio::Worker::Worker(Gobio & processor, Lattice & lattice) :
    LatticeWorker(lattice), processor_(processor)
{ }

void Gobio::Worker::doRun() {
    processor_.parse(lattice_);
}

std::string Gobio::doInfo() {
    return "gobio parser";
}

Gobio::Gobio(std::string rulesPath) : rulesPath_(rulesPath) { }

void Gobio::parse(Lattice & lattice) {

    AnnotationItemManager & aim = lattice.getAnnotationItemManager();

    Combinator combinator(aim);

    combinator.add_rules(rulesPath_);

    AV_AI_Converter av_ai_converter(
        aim,
        combinator.get_symbol_registrar(),
        combinator.get_attribute_registrar(),
        true
    );

    Chart ch(lattice, av_ai_converter);
    Agenda agenda;
    Parser parser(ch, combinator, agenda);

    parser.run();

    boost::shared_ptr< longest_left_to_right_chooser<Chart, Combinator> > chr(
        new longest_left_to_right_chooser<Chart, Combinator>()
    );

    std::vector<Combinator::rule_holder> local_rules;
    std::vector<Edge> choosen_edges = chr->go(ch, combinator, local_rules);

    LayerTagCollection tagParse
        = lattice.getLayerTagManager().createTagCollectionFromList(
            boost::assign::list_of("gobio")("parse")
        );


    BOOST_FOREACH(Edge e, choosen_edges) {
        lattice.addEdge(
            lattice.getEdgeSource(e),
            lattice.getEdgeTarget(e),
            lattice.getEdgeAnnotationItem(e),
            tagParse,
            lattice.getEdgePartitions(e).front().getSequence()
        );
    }

}

double Gobio::doGetQualityScore(
    const boost::program_options::variables_map & /*options*/) const {

    return -9999;
}
