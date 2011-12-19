#ifndef BY_SPACES_CUTTER_HDR
#define BY_SPACES_CUTTER_HDR


#include "annotation_item.hpp"
#include "cutter.hpp"


class BySpacesCutter : public Cutter {
public:
    virtual ~BySpacesCutter();

private:
    virtual AnnotationItem doCutOff(const std::string& text, size_t& positionInText);
    virtual AnnotationItem doCutOff(const StringFrag& text, size_t& positionInText);

    template <class StringType>
    AnnotationItem performCutOff(const StringType& text, size_t& positionInText);

    virtual void doReset();
    virtual size_t doSegmentLengthHardLimit();
    virtual size_t doSegmentLengthSoftLimit();
    virtual std::list<std::string> doLayerTags();
};


template <class StringType>
AnnotationItem BySpacesCutter::performCutOff(const StringType& text, size_t& positionInText) {
    size_t spacePosition = text.find(' ', positionInText);

    if (spacePosition == positionInText) {
        StringFrag spaceSF(
            text,
            positionInText,
            1
        );
        ++positionInText;
        return AnnotationItem("blank", spaceSF);
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
        return AnnotationItem("word", tokenSF);
    }
}


#endif
