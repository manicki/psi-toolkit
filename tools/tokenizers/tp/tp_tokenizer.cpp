#include "tp_tokenizer.hpp"
#include "tp_token_cutter.hpp"

#include "logging.hpp"


Annotator* TpTokenizer::Factory::doCreateAnnotator(
    boost::program_options::variables_map options) {
    return new TpTokenizer();
}

boost::program_options::options_description TpTokenizer::Factory::doOptionsHandled() {
    return boost::program_options::options_description();
}

std::string TpTokenizer::Factory::doGetName() {
    return "tp-tokenizer";
}

std::list<std::list<std::string> > TpTokenizer::Factory::doRequiredLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::list<std::string> > TpTokenizer::Factory::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> TpTokenizer::Factory::doProvidedLayerTags() {
    std::list<std::string> layerTags;
    layerTags.push_back("token");
    return layerTags;
}

LatticeWorker* TpTokenizer::doCreateLatticeWorker(Lattice& lattice) {
    return new Worker(*this, lattice);
}

TpTokenizer::Worker::Worker(Processor& processor, Lattice& lattice):
    LatticeWorker(lattice), processor_(processor) {
}

void TpTokenizer::Worker::doRun() {
    DEBUG("starting tp tokenizer...");

    LayerTagMask symbolMask = lattice_.getLayerTagManager().getMask("symbol");

    TpTokenCutter tokenCutter;

    lattice_.runCutter(tokenCutter, symbolMask);
}

std::string TpTokenizer::doInfo() {
    return "tp tokenizer";
}
