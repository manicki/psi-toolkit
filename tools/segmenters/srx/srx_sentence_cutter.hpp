#ifndef SRX_SENTENCE_CUTTER_HDR
#define SRX_SENTENCE_CUTTER_HDR

#include "annotation_item.hpp"
#include "cutter.hpp"

class SrxSentenceCutter : public Cutter {
public:
    TpTokenCutter(TPBasicTokenizerRuleSet& ruleSet);

private:
    virtual AnnotationItem doCutOff(const std::string& text, size_t& positionInText);
    virtual int doMaximumFragmentLength();
    virtual std::list<std::string> doLayerTags();

    AnnotationItem defaultToken(const std::string& text, size_t& positionInText);

    TPBasicTokenizerRuleSet& ruleSet_;
    const static std::string DEFAULT_CATEGORY;
};

#endif
