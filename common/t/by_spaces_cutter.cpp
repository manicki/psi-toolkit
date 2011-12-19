#include "by_spaces_cutter.hpp"

AnnotationItem BySpacesCutter::doCutOff(const std::string& text, size_t& positionInText) {
    return performCutOff(text, positionInText);
}

AnnotationItem BySpacesCutter::doCutOff(const StringFrag& text, size_t& positionInText) {
    return performCutOff(text, positionInText);
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
