#include "link_parser.hpp"


Annotator* LinkParser::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& /*options*/) {
    return new LinkParser();
}

void LinkParser::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription) {
    optionsDescription.add_options()
        ("option",
        boost::program_options::value<std::string>()->default_value("default_value"),
        "option_description")
        ;
}

std::string LinkParser::Factory::doGetName() const {
    return "link-parser";
}

boost::filesystem::path LinkParser::Factory::doGetFile() const {
    return __FILE__;
}

std::list<std::list<std::string> > LinkParser::Factory::doRequiredLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::list<std::string> > LinkParser::Factory::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> LinkParser::Factory::doProvidedLayerTags() {
    std::list<std::string> layerTags;
    layerTags.push_back("parse");
    return layerTags;
}

LatticeWorker* LinkParser::doCreateLatticeWorker(Lattice& lattice) {
    return new Worker(*this, lattice);
}

LinkParser::Worker::Worker(LinkParser& processor, Lattice& lattice) :
    LatticeWorker(lattice), processor_(processor)
{
    //TODO
}

void LinkParser::Worker::doRun() {
    processor_.parse(lattice_);
}

std::string LinkParser::doInfo() {
    return "link grammar parser";
}

LinkParser::LinkParser() {
    //TODO
}


void LinkParser::parse(Lattice &lattice) {
    //TODO
}
