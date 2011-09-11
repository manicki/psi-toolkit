#include "srx_segmenter.hpp"

#include <boost/function_output_iterator.hpp>

#include "logging.hpp"
#include "config.h"

#include "srx_rules.hpp"

#include "annotation_item.hpp"
#include "cutter.hpp"
#include "escaping.hpp"

Annotator* SrxSegmenter::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options) {
    std::string lang = options["lang"].as<std::string>();

    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);

    std::string rulesFileSpec = options["rules"].as<std::string>();

    boost::filesystem::path rules
        = fileFetcher.getOneFile(rulesFileSpec);

    return new SrxSegmenter(lang, rules);
}

boost::program_options::options_description SrxSegmenter::Factory::doOptionsHandled() {

    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("lang", boost::program_options::value<std::string>(), "language")
        ("rules",
         boost::program_options::value<std::string>()
         ->default_value(DEFAULT_RULE_FILE_SPEC),
         "rule file")
        ;

    return optionsDescription;
}

std::string SrxSegmenter::Factory::doGetName() {
    return "srx-segmenter";
}

std::list<std::list<std::string> > SrxSegmenter::Factory::doRequiredLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::list<std::string> > SrxSegmenter::Factory::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> SrxSegmenter::Factory::doProvidedLayerTags() {
    std::list<std::string> layerTags;
    layerTags.push_back("sentence");
    return layerTags;
}

const std::string SrxSegmenter::Factory::DEFAULT_RULE_FILE_SPEC
= "%ITSDATA%/%LANG%/segmentation.srx";


class RuleProcessor {
public:
    RuleProcessor(SrxSegmenter& segmenter)
        :segmenter_(segmenter) {
    }

    void operator()(const SrxRule& srxRule) {
        segmenter_.processRule_(srxRule);
    }

    private:
    SrxSegmenter& segmenter_;

};


void SrxSegmenter::processRule_(const SrxRule& srxRule) {
    boost::shared_ptr<PerlRegExp> ruleRegexp(
        new PerlRegExp(makeRegexpPart(srxRule)));

    if (srxRule.isBreakable()) {
        size_t nbOfNonBreakingRules = nonBreakingRules_.size();

        breakingRules_.push_back(
            BreakingRuleInfo(ruleRegexp, nbOfNonBreakingRules));
    }
    else
        nonBreakingRules_.push_back(ruleRegexp);
}

std::string SrxSegmenter::makeRegexp_(const SrxRule& srxRule) {
    return
        std::string("(")
        + makeAllParensNonCapturing_(srxRule.getBeforeBreak())
        + std::string(")(?:")
        + makeAllParensNonCapturing_(srxRule.getAfterBreak())
        + std::string(")");
}

std::string SrxSegmenter::makeAllParensNonCapturing_(const std::string& pattern) {
    size_t pos = 0;
    std::string ret = pattern;
    while ( (pos = ret.find("(", pos)) != std::string::npos ) {
        if (!Escaping::isEscaped(ret, pos)
            && !(pos + 1 < ret.length() && ret[pos+1] == '?')) {
            ret.replace(pos+1, 0, "?:");
            pos += 2;
        }

        ++pos;
    }

    return ret;
}

SrxSegmenter::SrxSegmenter(
    const std::string& lang,
    boost::filesystem::path rules)
    :firstRule_(true), nbBreakingRules_(0U) {

    SrxRulesReader ruleReader(rules, lang);
    RuleProcessor ruleProc(*this);

    ruleReader.getRules(boost::make_function_output_iterator(ruleProc));
    finish_();
}

LatticeWorker* SrxSegmenter::doCreateLatticeWorker(Lattice& lattice) {
    return new Worker(*this, lattice);
}

SrxSegmenter::Worker::Worker(Processor& processor, Lattice& lattice):
    LatticeWorker(lattice), processor_(processor) {
}


class SrxSentenceCutter : public Cutter {
public:
    SrxSentenceCutter(SrxSegmenter& segmenter)
        :segmenter_(segmenter) {
    }

private:
    const static std::string DEFAULT_SENTENCE_CATEGORY;

    virtual AnnotationItem doCutOff(const std::string& text, size_t& positionInText) {
        PerlStringPiece currentText(text.c_str() + positionInText);
        size_t textLen = text.length() - positionInText;

        size_t minBreakPoint = std::string::npos;

        BOOST_FOREACH(breakingRuleInfo& ruleInfo, breakingRules_) {
            size_t ruleBreakPoint = updateIndex_(ruleInfo.breakingRules, positionInText);

            if (ruleBreakPoint != std::string::npos
                && (ruleBreakPoint < minBreakPoint || minBreakPoint == std::string::npos))
                minBreakPoint = ruleBreakPoint;
        }

        if (minBreakPoint != std::string::npos) {
            size_t currentPosition = positionInText;
            size_t sentenceLength = textLen - currentText.size();

            positionInText += sentenceLength;

            return AnnotationItem(
                DEFAULT_SENTENCE_CATEGORY,
                text.substr(currentPosition, sentenceLength));
        }
        else {
            size_t currentPosition = positionInText;
            positionInText = std::string::npos;
            return AnnotationItem(
                DEFAULT_SENTENCE_CATEGORY,
                text.substr(currentPosition));
        }

    }

    virtual int doMaximumFragmentLength() {
        return 1000;
    }

    virtual std::list<std::string> doLayerTags() {
        std::list<std::string>  tags;
        tags.push_back(std::string("segmenter"));

        return tags;
    }

    SrxSegmenter& segmenter_;

};

const std::string SrxSentenceCutter::DEFAULT_SENTENCE_CATEGORY="sen";

void SrxSegmenter::Worker::doRun() {
    DEBUG("starting srx segmenter...");

    LayerTagMask symbolMask = lattice_.getLayerTagManager().getMask("symbol");

    SrxSentenceCutter sentenceCutter(dynamic_cast<SrxSegmenter&>(processor_));

    lattice_.runCutter(sentenceCutter, symbolMask);
}

std::string SrxSegmenter::doInfo() {
    return "SRX segmenter";
}
