#include "gobio.hpp"


Annotator* Gobio::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options
) {
    return new Gobio();
}

void Gobio::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription
) {
    optionsDescription.add_options()
        ("option",
        boost::program_options::value<std::string>()->default_value("default_value"),
        "option_description")
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

LatticeWorker* Gobio::doCreateLatticeWorker(Lattice& lattice) {
    return new Worker(*this, lattice);
}

Gobio::Worker::Worker(Gobio& processor, Lattice& lattice) :
    LatticeWorker(lattice), processor_(processor)
{
    //TODO
}

void Gobio::Worker::doRun() {
    //TODO
}

std::string Gobio::doInfo() {
    return "gobio parser";
}

Gobio::Gobio() {
    //TODO
}

bool Gobio::parse(Lattice &lattice) {
    //TODO
    return true;
}
