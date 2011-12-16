#include "tp_token_cutter.hpp"

#include "logging.hpp"
#include "string_helpers.hpp"

TpTokenCutter::TpTokenCutter(TPBasicTokenizerRuleSet& ruleSet):ruleSet_(ruleSet) {
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
    return 1000;
}

size_t TpTokenCutter::doSegmentLengthSoftLimit() {
    return 950;
}

std::list<std::string > TpTokenCutter::doLayerTags() {
    std::list<std::string>  tags;
    tags.push_back(std::string("token"));

    return tags;
}

const std::string TpTokenCutter::DEFAULT_CATEGORY = "X";
