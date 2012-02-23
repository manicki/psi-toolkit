#include "gobio.hpp"


Annotator* Gobio::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options) {

    std::string lang = options["lang"].as<std::string>();
    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);

    boost::shared_ptr<Combinator> combinatorPtr = boost::shared_ptr<Combinator>(new Combinator());

    if (options.count("rules")) {
        std::string rulesFilename = options["rules"].as<std::string>();
        boost::filesystem::path rulesPath =
            fileFetcher.getOneFile(rulesFilename);
        std::string rulesPathString = rulesPath.string();
        combinatorPtr->add_rules(rulesPathString);
    }

    return new Gobio(combinatorPtr);
}

void Gobio::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription) {
    optionsDescription.add_options()
        ("rules",
        boost::program_options::value<std::string>()->default_value(DEFAULT_RULE_FILE),
        "rules file")
        ;
}

std::string Gobio::Factory::doGetName() {
    return "gobio";
}

boost::filesystem::path Gobio::Factory::doGetFile() {
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
    layerTags.push_back("parse");
    return layerTags;
}

const std::string Gobio::Factory::DEFAULT_RULE_FILE
    = "%ITSDATA%/%LANG%/rules.g";

LatticeWorker* Gobio::doCreateLatticeWorker(Lattice& lattice) {
    return new Worker(*this, lattice);
}

Gobio::Worker::Worker(Gobio& processor, Lattice& lattice) :
    LatticeWorker(lattice), processor_(processor)
{
    //TODO
}

void Gobio::Worker::doRun() {
    processor_.parse(lattice_);
}

std::string Gobio::doInfo() {
    return "gobio parser";
}

Gobio::Gobio(boost::shared_ptr<Combinator> combinatorPtr) : combinator_(combinatorPtr) {
    //TODO
}


void Gobio::parse(Lattice &lattice) {

    Chart ch(lattice);
    Agenda agenda;
    Parser parser(ch, *combinator_, agenda);

    parser.run();

    //DUMMY IMPLEMENTATION (for testing in psi-pipe)
    /*
    AnnotationItem aiGobio("parsed by GOBIO");
    LayerTagCollection
        tagParse = lattice.getLayerTagManager().createSingletonTagCollection("parse");
    lattice.addEdge(
        lattice.getFirstVertex(),
        lattice.getLastVertex(),
        aiGobio,
        tagParse
    );
    // */
}
