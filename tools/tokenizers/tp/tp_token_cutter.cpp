#include "tp_token_cutter.hpp"

#include "logging.hpp"
#include "string_helpers.hpp"

TpTokenCutter::TpTokenCutter(TPBasicTokenizerRuleSet& ruleSet):ruleSet_(ruleSet) {
}

AnnotationItem TpTokenCutter::doCutOff(const std::string& text, size_t& positionInText) {

    size_t len = text.length() - positionInText;
    PerlStringPiece textSp(text.data()+positionInText, len);

    for (size_t i = 0; i < ruleSet_.getRegexCount(); ++i) {
        PerlRegExp* regexp = ruleSet_.getRegex(i);

        if (PerlRegExp::Consume(&textSp, *regexp)) {

            size_t tokenLength = len - textSp.size();

            size_t originalPositionInText = positionInText;
            positionInText += tokenLength;

            return AnnotationItem(
                ruleSet_.getRegexCategory(i),
                StringFrag(text, originalPositionInText, tokenLength)); //TODO
        }
    }

    return defaultToken(text, positionInText);
}

AnnotationItem TpTokenCutter::doCutOff(const StringFrag& text, size_t& positionInText) {
    //TODO
    return doCutOff(text.str(), positionInText);
}

void TpTokenCutter::doReset() {
}

AnnotationItem TpTokenCutter::defaultToken(const std::string& text, size_t& positionInText) {

    size_t oldPosition = positionInText;
    size_t len = symbolLength(text, oldPosition);
    positionInText += len;

    return AnnotationItem(
        DEFAULT_CATEGORY,
        StringFrag(text, oldPosition, len)); //TODO
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
