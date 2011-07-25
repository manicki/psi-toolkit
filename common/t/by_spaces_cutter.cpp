#include "by_spaces_cutter.hpp"

AnnotationItem BySpacesCutter::doCutOff(const std::string& text, size_t& positionInText) {
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

int BySpacesCutter::doMaximumFragmentLength() {
    return 1000;
}

std::list<std::string> BySpacesCutter::doLayerTags() {
    std::list<std::string> tags;
    tags.push_back(std::string("token"));

    return tags;
}
