#include "tp_token_cutter.hpp"

#include "logging.hpp"
#include "string_helpers.hpp"
#include "layer_tag_manager.hpp"

#include <boost/assign.hpp>

TpTokenCutter::TpTokenCutter(
    const std::string& langCode,
    TPBasicTokenizerRuleSet& ruleSet,
    size_t hardLimit,
    size_t softLimit)
    :langCode_(langCode), ruleSet_(ruleSet), hardLimit_(hardLimit), softLimit_(softLimit) {
}

AnnotationItem TpTokenCutter::doCutOff(const std::string& text, size_t& positionInText) {
    return performCutOff(text, positionInText);
}

AnnotationItem TpTokenCutter::doCutOff(const StringFrag& text, size_t& positionInText) {
    return performCutOff(text, positionInText);
}

void TpTokenCutter::doReset() {
}

size_t TpTokenCutter::doSegmentLengthHardLimit() {
    return hardLimit_;
}

size_t TpTokenCutter::doSegmentLengthSoftLimit() {
    return softLimit_;
}

std::list<std::string> TpTokenCutter::doLayerTags() {
    return boost::assign::list_of
        (std::string("token"))
        (LayerTagManager::getLanguageTag(langCode_));
}

const std::string TpTokenCutter::DEFAULT_CATEGORY = "X";
