#include "tp_token_cutter.hpp"

#include "logging.hpp"

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
                text.substr(originalPositionInText, tokenLength));
        }
    }

    return defaultToken(text, positionInText);
}

AnnotationItem TpTokenCutter::doCutOff(const StringFrag& text, size_t& positionInText) {
    // TODO
    return doCutOff(text.str(), positionInText);
}

AnnotationItem TpTokenCutter::defaultToken(const std::string& text, size_t& positionInText) {
    return AnnotationItem(
        DEFAULT_CATEGORY,
        text.substr(positionInText++, 1));
}

int TpTokenCutter::doMaximumFragmentLength() {
    return 1000;
}

std::list<std::string > TpTokenCutter::doLayerTags() {
    std::list<std::string>  tags;
    tags.push_back(std::string("token"));

    return tags;
}

const std::string TpTokenCutter::DEFAULT_CATEGORY = "X";
