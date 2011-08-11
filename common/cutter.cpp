#include "cutter.hpp"

AnnotationItem Cutter::cutOff(const std::string& text, size_t& positionInText) {
    return doCutOff(text, positionInText);
}

int Cutter::maximumFragmentLength() {
    return doMaximumFragmentLength();
}

std::list<std::string> Cutter::layerTags() {
    return doLayerTags();
}

Cutter::~Cutter() {
}

