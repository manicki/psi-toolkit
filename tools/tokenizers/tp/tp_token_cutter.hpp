#ifndef TP_TOKEN_CUTTER_HDR
#define TP_TOKEN_CUTTER_HDR

#include "annotation_item.hpp"
#include "cutter.hpp"
#include "tp_basic_tokenizer_rule_set.hpp"

class TpTokenCutter : public Cutter {
public:
    TpTokenCutter(TPBasicTokenizerRuleSet& ruleSet);

private:
    virtual AnnotationItem doCutOff(const std::string& text, size_t& positionInText);
    virtual AnnotationItem doCutOff(const StringFrag& text, size_t& positionInText);
    virtual void doReset();
    virtual size_t doSegmentLengthHardLimit();
    virtual size_t doSegmentLengthSoftLimit();
    virtual std::list<std::string> doLayerTags();

    AnnotationItem defaultToken(const std::string& text, size_t& positionInText);

    TPBasicTokenizerRuleSet& ruleSet_;
    const static std::string DEFAULT_CATEGORY;
};

#endif
