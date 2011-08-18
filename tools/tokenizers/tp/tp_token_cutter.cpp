#include "tp_token_cutter.hpp"

AnnotationItem TpTokenCutter::doCutOff(const std::string& text, size_t& positionInText) {
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

int TpTokenCutter::doMaximumFragmentLength() {
    return 1000;
}

std::list<std::string > TpTokenCutter::doLayerTags() {
    std::list<std::string>  tags;
    tags.push_back(std::string("token"));

    return tags;
}
