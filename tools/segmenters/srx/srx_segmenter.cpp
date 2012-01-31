#include "srx_segmenter.hpp"

#include <cassert>
#include <boost/function_output_iterator.hpp>

#include "logging.hpp"
#include "config.hpp"

#include "srx_rules.hpp"

#include "annotation_item.hpp"
#include "cutter.hpp"
#include "escaping.hpp"

#include "cutter_annotator_options.hpp"

Annotator* SrxSegmenter::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options) {
    std::string lang = options["lang"].as<std::string>();

    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);

    std::string rulesFileSpec = options["rules"].as<std::string>();

    boost::filesystem::path rules
        = fileFetcher.getOneFile(rulesFileSpec);

    size_t hardLimit = options["sentence-length-hard-limit"].as<size_t>();
    size_t softLimit = options["sentence-length-soft-limit"].as<size_t>();

    bool cascade = options["cascade"].as<bool>();

    return new SrxSegmenter(lang, rules, hardLimit, softLimit, cascade);
}

void SrxSegmenter::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription) {

    optionsDescription.add_options()
        ("rules",
         boost::program_options::value<std::string>()
         ->default_value(DEFAULT_RULE_FILE_SPEC),
         "rule file")
        ("cascade", 
         boost::program_options::bool_switch()->default_value(false),
         "force cascade mode")
        ;

    addCutterAnnotatorOptions(
        optionsDescription,
        DEFAULT_HARD_LIMIT,
        DEFAULT_SOFT_LIMIT,
        "sentence");

}

std::string SrxSegmenter::Factory::doGetName() {
    return "srx-segmenter";
}

boost::filesystem::path SrxSegmenter::Factory::doGetFile() {
    return __FILE__;
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

const size_t SrxSegmenter::Factory::DEFAULT_HARD_LIMIT = 1000;
const size_t SrxSegmenter::Factory::DEFAULT_SOFT_LIMIT = 600;

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
        new PerlRegExp(makeRegexp_(srxRule)));

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
        std::string("(?:")
        + makeAllParensNonCapturing_(srxRule.getBeforeBreak())
        + std::string(")(")
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
    boost::filesystem::path rules,
    size_t hardLimit,
    size_t softLimit,
    bool cascade):
    hardLimit_(hardLimit), softLimit_(softLimit) {

    SrxRulesReader ruleReader(rules, lang, cascade);
    RuleProcessor ruleProc(*this);

    ruleReader.getRules(boost::make_function_output_iterator(ruleProc));
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
        :segmenter_(segmenter),
         breakingRuleApplications_(segmenter.breakingRules_.size()),
         nonBreakingRuleApplications_(segmenter.nonBreakingRules_.size()) {
    }

private:
    const static std::string DEFAULT_SENTENCE_CATEGORY;

    virtual AnnotationItem doCutOff(const std::string& text, size_t& positionInText) {
        return performCutOff(text, positionInText);
    }

    virtual AnnotationItem doCutOff(const StringFrag& text, size_t& positionInText) {
        return performCutOff(text, positionInText);
    }

    template <class StringType>
    AnnotationItem performCutOff(const StringType& text, size_t& positionInText) {
        size_t nearestBreakPoint = 0;
        size_t nearestRuleIndex = 0;
        size_t minBreakPoint =
            positionInText + utf8::unchecked::sequence_length(text.data() + positionInText);
        bool candidateFound = false;
        bool candidateAccepted = false;

        assert (segmenter_.breakingRules_.size() == breakingRuleApplications_.size());
        assert (segmenter_.nonBreakingRules_.size() == nonBreakingRuleApplications_.size());

        do {
            candidateFound = false;

            for (size_t i = 0; i < segmenter_.breakingRules_.size(); ++i) {
                size_t ruleBreakPoint = updateBreakingRuleIndex_(i,
                                                                 minBreakPoint,
                                                                 text,
                                                                 positionInText);

                assert (ruleBreakPoint == std::string::npos
                        || ruleBreakPoint >= minBreakPoint);

                if (ruleBreakPoint != std::string::npos
                    && (!candidateFound || ruleBreakPoint < nearestBreakPoint)) {
                    INFO("CANDIDATE " << i << " " << ruleBreakPoint
                         << " " << nearestBreakPoint
                         << " " << minBreakPoint
                         << " " << breakingRuleApplications_[i].startingPosition
                         << " " << breakingRuleApplications_[i].breakingPosition);

                    nearestBreakPoint = ruleBreakPoint;
                    nearestRuleIndex = i;
                    candidateFound = true;
                }
            }

            if (candidateFound) {
                candidateAccepted = checkBreakPoint_(
                    nearestBreakPoint,
                    text,
                    positionInText,
                    segmenter_.breakingRules_[nearestRuleIndex].nbOfApplicableNonBreakingRules);

                if (!candidateAccepted)
                    minBreakPoint =
                        nearestBreakPoint
                        + utf8::unchecked::sequence_length(text.data() + nearestBreakPoint);
            }

            assert (!(!candidateFound && candidateAccepted));
        } while (candidateFound && !candidateAccepted);

        if (candidateAccepted) {
            assert (candidateFound);

            size_t currentPosition = positionInText;

            assert (nearestBreakPoint > currentPosition);
            size_t sentenceLength = nearestBreakPoint - currentPosition;

            positionInText += sentenceLength;

            return AnnotationItem(
                DEFAULT_SENTENCE_CATEGORY,
                StringFrag(text, currentPosition, sentenceLength));
        }
        else {
            size_t currentPosition = positionInText;
            positionInText = std::string::npos;
            return AnnotationItem(
                DEFAULT_SENTENCE_CATEGORY,
                StringFrag(text, currentPosition, text.length() - currentPosition));
        }
    }

    virtual void doReset() {
        BOOST_FOREACH(RuleApplication& ruleApplication, breakingRuleApplications_) {
            ruleApplication = RuleApplication();
        }

        BOOST_FOREACH(RuleApplication& ruleApplication, nonBreakingRuleApplications_) {
            ruleApplication = RuleApplication();
        }
    }

    virtual size_t doSegmentLengthHardLimit() {
        return segmenter_.hardLimit_;
    }

    virtual size_t doSegmentLengthSoftLimit() {
        return segmenter_.softLimit_;
    }

    virtual std::list<std::string> doLayerTags() {
        std::list<std::string>  tags;
        tags.push_back(std::string("segment"));

        return tags;
    }

    template <class StringType>
    size_t updateBreakingRuleIndex_(size_t breakingRuleIndex,
                                    size_t minBreakPoint,
                                    const StringType& text,
                                    size_t positionInText) {
        SrxSegmenter::BreakingRuleInfo& ruleInfo = segmenter_.breakingRules_[breakingRuleIndex];

        return updatePosition_(ruleInfo.breakingRule,
                               breakingRuleApplications_[breakingRuleIndex],
                               minBreakPoint,
                               text,
                               positionInText);
    }

    template <class StringType>
    bool checkBreakPoint_(size_t breakPoint,
                          const StringType& text,
                          size_t positionInText,
                          size_t nbOfRulesToCheck) {

        assert (nbOfRulesToCheck <= nonBreakingRuleApplications_.size());

        for (size_t i = 0; i < nbOfRulesToCheck; ++i) {
            size_t ruleNonBreakPoint = updateNonBreakingRuleIndex_(i,
                                                                   breakPoint,
                                                                   text,
                                                                   positionInText);

            if (ruleNonBreakPoint != std::string::npos
                && ruleNonBreakPoint == breakPoint)
                return false;
        }

        return true;
    }

    template <class StringType>
    size_t updateNonBreakingRuleIndex_(size_t breakingRuleIndex,
                                       size_t minBreakPoint,
                                       const StringType& text,
                                       size_t positionInText) {
        return updatePosition_(segmenter_.nonBreakingRules_[breakingRuleIndex],
                               nonBreakingRuleApplications_[breakingRuleIndex],
                               minBreakPoint,
                               text,
                               positionInText);
    }

    SrxSegmenter& segmenter_;

    struct RuleApplication {
        size_t startingPosition;
        size_t breakingPosition;

        RuleApplication()
            :startingPosition(0U), breakingPosition(std::string::npos) {
        }
    };

    std::vector<RuleApplication> breakingRuleApplications_;
    std::vector<RuleApplication> nonBreakingRuleApplications_;

    template <class StringType>
    size_t updatePosition_(boost::shared_ptr<PerlRegExp> regexp,
                           RuleApplication& ruleApplication,
                           size_t minBreakPoint,
                           const StringType& text,
                           size_t positionInText) {

        if (ruleApplication.startingPosition < positionInText)
            ruleApplication.startingPosition = positionInText;

        while (ruleApplication.startingPosition < text.length()
               && (ruleApplication.breakingPosition == std::string::npos
                   || ruleApplication.breakingPosition < minBreakPoint)) {
            PerlStringPiece currentText(text.data() + ruleApplication.startingPosition);
            int originalLength = currentText.size();
            PerlStringPiece fragFound;

            if (PerlRegExp::FindAndConsume(&currentText, *regexp.get(), &fragFound)) {
                assert (originalLength > currentText.size());
                int lengthDiff = originalLength - currentText.size();

                ruleApplication.breakingPosition =
                    ruleApplication.startingPosition + (lengthDiff - fragFound.size());

                assert (ruleApplication.breakingPosition >= ruleApplication.startingPosition);

                ruleApplication.startingPosition +=
                    utf8::unchecked::sequence_length(
                        text.data() + ruleApplication.startingPosition);
            }
            else {
                ruleApplication.breakingPosition = std::string::npos;
                ruleApplication.startingPosition = text.length();
            }
        }

        if (ruleApplication.breakingPosition < minBreakPoint)
            ruleApplication.breakingPosition = std::string::npos;

        return ruleApplication.breakingPosition;
    }
};

const std::string SrxSentenceCutter::DEFAULT_SENTENCE_CATEGORY="sen";

void SrxSegmenter::Worker::doRun() {
    DEBUG("starting srx segmenter...");

    LayerTagMask symbolMask = lattice_.getLayerTagManager().getMask("symbol");
    LayerTagMask textMask = lattice_.getLayerTagManager().getMask("frag");

    SrxSentenceCutter sentenceCutter(dynamic_cast<SrxSegmenter&>(processor_));

    lattice_.runCutter(sentenceCutter, symbolMask, textMask);
}

std::string SrxSegmenter::doInfo() {
    return "SRX segmenter";
}
