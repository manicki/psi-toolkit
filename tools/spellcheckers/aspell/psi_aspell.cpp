#include "psi_aspell_adapter.hpp"
#include "psi_aspell.hpp"

#include "logging.hpp"
#include "config.hpp"

Annotator* PSIAspell::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options) {

    std::string lang = options["lang"].as<std::string>();

    // @todo
    return new PSIAspell();
}

void PSIAspell::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription) {

    // @todo

}

std::string PSIAspell::Factory::doGetName() {
    return "aspell";
}

boost::filesystem::path PSIAspell::Factory::doGetFile() {
    return __FILE__;
}

std::list<std::list<std::string> > PSIAspell::Factory::doRequiredLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::list<std::string> > PSIAspell::Factory::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> PSIAspell::Factory::doProvidedLayerTags() {
    std::list<std::string> layerTags;
    layerTags.push_back("token");
    return layerTags;
}

std::string PSIAspell::Factory::doGetContinuation(
    const boost::program_options::variables_map& /* options */) const {
    return "simple-writer";
}

PSIAspell::PSIAspell() {
    // @todo
}

LatticeWorker* PSIAspell::doCreateLatticeWorker(Lattice& lattice) {
    return new Worker(*this, lattice);
}

PSIAspell::Worker::Worker(Processor& processor, Lattice& lattice):
    LatticeWorker(lattice), processor_(processor) {
}

void PSIAspell::Worker::doRun() {
    DEBUG("starting aspell...");

    PSIAspellAdapter aspellAdapter;
    // @todo
}

std::string PSIAspell::doInfo() {
    return "Aspell";
}
