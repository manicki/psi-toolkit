#include "by_spaces_cutter.hpp"

AnnotationItem BySpacesCutter::doCutOff(const std::string& text, size_t& positionInText) {
    size_t spacePosition = text.find(' ', positionInText);

    if (spacePosition == positionInText) {
        ++positionInText;
        return AnnotationItem("blank", " ");
    }
    else {
        std::string token = text.substr(positionInText,
                                        (spacePosition == std::string::npos ?
                                         std::string::npos :
                                         spacePosition - positionInText));

        positionInText = spacePosition;
        return AnnotationItem("word", token);
    }
}

AnnotationItem BySpacesCutter::doCutOff(const StringFrag& text, size_t& positionInText) {
    // TODO
    return doCutOff(text.str(), positionInText);
    /*
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
                ? std::string::npos
                : spacePosition - positionInText
            )
        );
        positionInText = spacePosition;
        return AnnotationItem("word", tokenSF);
    }
    */
}

void BySpacesCutter::doReset() {
}

size_t BySpacesCutter::doSegmentLengthHardLimit() {
    return 1000;
}

size_t BySpacesCutter::doSegmentLengthSoftLimit() {
    return 0;
}

std::list<std::string> BySpacesCutter::doLayerTags() {
    std::list<std::string> tags;
    tags.push_back(std::string("token"));

    return tags;
}

BySpacesCutter::~BySpacesCutter() {
}
