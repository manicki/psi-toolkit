#include "tp_tokenizer.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/assign/list_of.hpp>

#include "tp_token_cutter.hpp"

#include "logging.hpp"
#include "config.hpp"

#include "tp_basic_tokenizer_rule_set.hpp"
#include "cutter_annotator_options.hpp"

Annotator* TpTokenizer::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options) {
    std::string lang = options["lang"].as<std::string>();

    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);

    std::string rulesFileSpec = options["rules"].as<std::string>();

    boost::filesystem::path rules
        = fileFetcher.getOneFile(rulesFileSpec);

    std::map<std::string, std::string> rawMapping =
        parseMappingOption_(options["mapping"].as<std::string>(), lang);

    std::map<std::string, boost::filesystem::path> mapping =
        substituteMapping_(rawMapping, fileFetcher);

    size_t hardLimit = options["token-length-hard-limit"].as<size_t>();
    size_t softLimit = options["token-length-soft-limit"].as<size_t>();

    return new TpTokenizer(lang, rules, mapping, hardLimit, softLimit);
}

void TpTokenizer::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription) {

    optionsDescription.add_options()
        ("rules",
         boost::program_options::value<std::string>()
         ->default_value(DEFAULT_RULE_FILE_SPEC),
         "rule file")
        ("mapping",
         boost::program_options::value<std::string>()
         ->default_value(DEFAULT_RULE_FILE_MAPPING),
         "mapping between include names and files")
        ;

    addCutterAnnotatorOptions(
        optionsDescription,
        DEFAULT_HARD_LIMIT,
        DEFAULT_SOFT_LIMIT,
        "token");

}

std::string TpTokenizer::Factory::doGetName() const {
    return "tp-tokenizer";
}

std::list<std::string> TpTokenizer::Factory::doGetAliases() {
    return boost::assign::list_of<std::string>
        (std::string("tp-tokeniser"))
        (std::string("tp-tokenise"))
        (std::string("tp-tokenize"));
}

boost::filesystem::path TpTokenizer::Factory::doGetFile() const {
    return __FILE__;
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

std::string TpTokenizer::Factory::doGetContinuation(
    const boost::program_options::variables_map& /* options */) const {

    return "simple-writer";
}

std::map<std::string, std::string> TpTokenizer::Factory::parseMappingOption_(
    const std::string& mappingOption,
    const std::string& lang) {

    std::map<std::string, std::string> mapping;

    std::vector<std::string> assignments;
    boost::split(assignments, mappingOption, boost::is_any_of(";"));

    for (std::vector<std::string>::const_iterator iter = assignments.begin();
         iter != assignments.end();
         ++iter) {

        size_t equalSignPos = (*iter).find('=');

        if (equalSignPos != std::string::npos) {
            std::string key = (*iter).substr(0, equalSignPos);
            std::string value = (*iter).substr(equalSignPos+1);

            // unfortunately, lang is referred in
            // an include name
            boost::algorithm::replace_all(
                key,
                "%LANG%",
                lang);

            mapping[key] = value;
        }
        else {
            ERROR("wrong mapping `" << (*iter) << "'");
        }
    }

    return mapping;
}

std::map<std::string, boost::filesystem::path>
TpTokenizer::Factory::substituteMapping_(
    const std::map<std::string, std::string>& rawMapping,
    const LangSpecificProcessorFileFetcher& fileFetcher) {

    std::map<std::string, boost::filesystem::path> mapping;

    for (std::map<std::string, std::string>::const_iterator iter
             = rawMapping.begin();
         iter != rawMapping.end();
         ++iter)
        mapping[iter->first] = fileFetcher.getOneFile(iter->second);

    return mapping;
}

const std::string TpTokenizer::Factory::DEFAULT_RULE_FILE_SPEC
= "%ITSDATA%/%LANG%/%LANG%.rgx";

const std::string TpTokenizer::Factory::DEFAULT_RULE_FILE_MAPPING
= "common=%ITSDATA%/common.rgx;abbrev_%LANG%=%ITSDATA%/%LANG%/abbrev.rgx";

const size_t TpTokenizer::Factory::DEFAULT_HARD_LIMIT = 1000;
const size_t TpTokenizer::Factory::DEFAULT_SOFT_LIMIT = 950;

TpTokenizer::TpTokenizer(
    const std::string& langCode,
    boost::filesystem::path rules,
    const std::map<std::string, boost::filesystem::path>& mapping,
    size_t hardLimit,
    size_t softLimit)
    :langCode_(langCode), hardLimit_(hardLimit), softLimit_(softLimit) {

    ruleSet_.reset(new TPBasicTokenizerRuleSet());

    std::map<std::string, std::string> pathMap;
    for (std::map<std::string, boost::filesystem::path>::const_iterator iter
             = mapping.begin();
         iter != mapping.end();
         ++iter)
        pathMap[iter->first] = iter->second.string();
    pathMap["main"] = rules.string();

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

    TpTokenizer& tpProcessor = dynamic_cast<TpTokenizer&>(processor_);

    LayerTagMask symbolMask = lattice_.getLayerTagManager().getMask("symbol");

    LayerTagMask textMask = lattice_.getLayerTagManager().getMask(
        lattice_.getLayerTagManager()
        .createSingletonTagCollectionWithLangCode("text", tpProcessor.langCode_));

    TpTokenCutter tokenCutter(
        tpProcessor.langCode_,
        *tpProcessor.ruleSet_.get(),
        tpProcessor.hardLimit_,
        tpProcessor.softLimit_);

    lattice_.runCutter(tokenCutter, symbolMask, textMask);
}

std::string TpTokenizer::doInfo() {
    return "tp tokenizer";
}
