#ifndef TP_TOKEN_CUTTER_HDR
#define TP_TOKEN_CUTTER_HDR

#include "annotation_item.hpp"
#include "cutter.hpp"
#include "tp_basic_tokenizer_rule_set.hpp"
#include "string_helpers.hpp"

class TpTokenCutter : public Cutter {
public:
    TpTokenCutter(
        TPBasicTokenizerRuleSet& ruleSet,
        size_t hardLimit,
        size_t softLimit);

private:
    template <typename StringType>
    AnnotationItem performCutOff(const StringType& text, size_t& positionInText);

    virtual AnnotationItem doCutOff(const std::string& text, size_t& positionInText);
    virtual AnnotationItem doCutOff(const StringFrag& text, size_t& positionInText);

    virtual void doReset();
    virtual size_t doSegmentLengthHardLimit();
    virtual size_t doSegmentLengthSoftLimit();
    virtual std::list<std::string> doLayerTags();

    template <typename StringType>
    AnnotationItem defaultToken(const StringType& text, size_t& positionInText);

    TPBasicTokenizerRuleSet& ruleSet_;
    size_t hardLimit_;
    size_t softLimit_;
    const static std::string DEFAULT_CATEGORY;
};


template <typename StringType>
AnnotationItem TpTokenCutter::performCutOff(const StringType& text, size_t& positionInText) {

    size_t len = text.length() - positionInText;
    PerlStringPiece textSp(text.data()+positionInText, len);

    for (size_t i = 0; i < ruleSet_.getRegexCount(); ++i) {
        PerlRegExp* regexp = ruleSet_.getRegex(i);

        if (PerlRegExp::Consume(&textSp, *regexp)) {
            size_t tokenLength = len - textSp.size();
            StringFrag sf(text, positionInText, tokenLength);
            positionInText += tokenLength;
            return AnnotationItem(ruleSet_.getRegexCategory(i), sf);
        }
    }

    return defaultToken(text, positionInText);
}


template <typename StringType>
AnnotationItem TpTokenCutter::defaultToken(const StringType& text, size_t& positionInText) {
    size_t len = symbolLength(text, positionInText);
    StringFrag sf(text, positionInText, len);
    positionInText += len;
    return AnnotationItem(DEFAULT_CATEGORY, sf);
}


#endif
