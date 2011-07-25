#include "tp_token_cutter.hpp"

AnnotationItem doCutOff(const std::string& text, size_t& positionInText) {
    size_t spacePosition = text.find(' ', positionInText);

    if (spacePosition == positionInText) {
        ++positionInText;
        return AnnotationItem("' '");
    }
    else {
        // could be string::npos and it will be ok
        std::string cat = std::string("'") + text.substr(positionInText, spacePosition) + std::string("'");
        positionInText = spacePosition;
        return AnnotationItem(cat);
    }
}

int doMaximumFragmentLength() {
    return 1000;
}
