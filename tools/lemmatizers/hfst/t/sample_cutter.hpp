#ifndef HFST_TEST_SAMPLE_CUTTER_HDR
#define HFST_TEST_SAMPLE_CUTTER_HDR

#include "annotation_item.hpp"
#include "cutter.hpp"
#include "layer_tag_manager.hpp"

class SampleCutter : public Cutter {
public:
    SampleCutter(const std::string &lang);
    virtual ~SampleCutter();

private:
    virtual AnnotationItem doCutOff(const std::string& text, size_t& positionInText);
    virtual AnnotationItem doCutOff(const StringFrag& text, size_t& positionInText);

    template <typename StringType>
    AnnotationItem performCutOff(const StringType& text, size_t& positionInText);

    virtual void doReset();
    virtual size_t doSegmentLengthHardLimit();
    virtual size_t doSegmentLengthSoftLimit();
    virtual std::list<std::string> doLayerTags();

    std::string lang_;
};

SampleCutter::SampleCutter(const std::string &lang) {
    lang_ = lang;
}

template <typename StringType>
AnnotationItem SampleCutter::performCutOff(const StringType& text, size_t& positionInText) {
    size_t spacePosition = text.find(' ', positionInText);

    if (spacePosition == positionInText) {
        StringFrag spaceSF(
            text,
            positionInText,
            1
        );
        ++positionInText;
        return AnnotationItem("B", spaceSF);
    }
    else {
        StringFrag tokenSF(
            text,
            positionInText,
            (spacePosition == std::string::npos
                ? text.length() - positionInText
                : spacePosition - positionInText
            )
        );
        positionInText = spacePosition;
        return AnnotationItem("T", tokenSF);
    }
}

AnnotationItem SampleCutter::doCutOff(const std::string& text, size_t& positionInText) {
    return performCutOff(text, positionInText);
}

AnnotationItem SampleCutter::doCutOff(const StringFrag& text, size_t& positionInText) {
    return performCutOff(text, positionInText);
}

void SampleCutter::doReset() {
}

size_t SampleCutter::doSegmentLengthHardLimit() {
    return 1000;
}

size_t SampleCutter::doSegmentLengthSoftLimit() {
    return 0;
}

std::list<std::string> SampleCutter::doLayerTags() {
    std::list<std::string> tags;
    tags.push_back(std::string("token"));
    tags.push_back(LayerTagManager::getLanguageTag(lang_));

    return tags;
}

SampleCutter::~SampleCutter() {
}

#endif
