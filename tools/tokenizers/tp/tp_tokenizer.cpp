#include "tp_tokenizer.hpp"

#include "tp_token_cutter.hpp"

#include "logging.hpp"
#include "config.h"

#include "tp_basic_tokenizer_rule_set.hpp"

Annotator* TpTokenizer::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options) {
    std::string lang = options["lang"].as<std::string>();

    return new TpTokenizer(lang);
}

boost::program_options::options_description TpTokenizer::Factory::doOptionsHandled() {

    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("lang", boost::program_options::value<std::string>(), "language")
        ;

    return optionsDescription;
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

TpTokenizer::TpTokenizer(const std::string& lang) {
    ruleSet_.reset(new TPBasicTokenizerRuleSet());

    std::map<std::string, std::string> pathMap;
    pathMap["main"] =
        std::string(ROOT_DIR "tools/tokenizers/tp/data/")
        + lang + "/" + lang + ".rgx";
    pathMap["common"] = ROOT_DIR "tools/tokenizers/tp/data/xx/xx.rgx";
    pathMap["abbrev_" + lang] =
        std::string(ROOT_DIR "tools/tokenizers/tp/data/")
        + lang + "/abbrev.rgx";

    std::list<std::string> paths;
    paths.push_back("main");

    ruleSet_->load(pathMap, paths);
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

    TpTokenCutter tokenCutter(*dynamic_cast<TpTokenizer&>(processor_).ruleSet_.get());

    lattice_.runCutter(tokenCutter, symbolMask);
}

std::string TpTokenizer::doInfo() {
    return "tp tokenizer";
}
