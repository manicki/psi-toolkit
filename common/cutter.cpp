#include "cutter.hpp"

#include <algorithm>

#include "string_helpers.hpp"
#include "logging.hpp"

AnnotationItem Cutter::cutOff(const std::string& text, size_t& positionInText) {
    if (shouldFragmentQueueBeUsed_())
        return getFirstItemInFragmentQueue_(text, positionInText);

    size_t prevPosition = positionInText;
    AnnotationItem item = doCutOff(text, positionInText);

    size_t realPositionInText =
        (positionInText == std::string::npos
         ? text.length()
         : positionInText);

    size_t segmentLength = realPositionInText - prevPosition;

    if (areLimitsBroken_(segmentLength)) {
        DEBUG("segment length limits broken [" << segmentLength << "]");

        positionInText = prevPosition;
        fragmentSegment_(item, text, positionInText, segmentLength);

        return getFirstItemInFragmentQueue_(text, positionInText);
    }

    return item;
}

AnnotationItem Cutter::cutOff(const StringFrag& text, size_t& positionInText) {
    return doCutOff(text, positionInText);
}

size_t Cutter::segmentLengthHardLimit() {
    return doSegmentLengthHardLimit();
}

size_t Cutter::segmentLengthSoftLimit() {
    return doSegmentLengthSoftLimit();
}

std::list<std::string> Cutter::layerTags() {
    return doLayerTags();
}

Cutter::~Cutter() {
}

bool Cutter::shouldFragmentQueueBeUsed_() {
    return !fragmentedSegmentsQueue_.empty();
}

AnnotationItem Cutter::getFirstItemInFragmentQueue_(
    const std::string& text, size_t& positionInText) {

    AnnotationItem item = fragmentedSegmentsQueue_.front();
    fragmentedSegmentsQueue_.pop();

    positionInText += item.getText().length();

    if (positionInText == text.length())
        positionInText = std::string::npos;

    return item;
}

bool Cutter::areLimitsBroken_(size_t segmentLength) {
    return
        isSoftLimitBroken_(segmentLength)
        || isHardLimitBroken_(segmentLength);
}

bool Cutter::isSoftLimitBroken_(size_t segmentLength) {
    return isSoftLimitSet_() && segmentLength > segmentLengthSoftLimit();
}

bool Cutter::isSoftLimitSet_() {
    return segmentLengthSoftLimit() != 0;
}

bool Cutter::isHardLimitBroken_(size_t segmentLength) {
    return isHardLimitSet_() && segmentLength > segmentLengthHardLimit();
}

bool Cutter::isHardLimitSet_() {
    return segmentLengthHardLimit() != 0;
}

void Cutter::fragmentSegment_(
    const AnnotationItem& item,
    const std::string& text,
    size_t positionInText,
    size_t segmentLength) {

    size_t cutPoint = positionInText;

    while (cutPoint < positionInText + segmentLength) {
        size_t currentLength = segmentLength - (cutPoint - positionInText);

        size_t softLimitCutPoint =
            isSoftLimitBroken_(currentLength)
            ? findSoftLimitCutPoint_(text, cutPoint, positionInText + segmentLength)
            : std::string::npos;

        size_t hardLimitCutPoint =
            isHardLimitBroken_(currentLength)
            ? findHardLimitCutPoint_(text, cutPoint)
            : std::string::npos;

        size_t prevCutPoint = cutPoint;

        if (softLimitCutPoint != std::string::npos && hardLimitCutPoint != std::string::npos)
            cutPoint = std::min(softLimitCutPoint, hardLimitCutPoint);
        else if (softLimitCutPoint != std::string::npos)
            cutPoint = softLimitCutPoint;
        else if (hardLimitCutPoint != std::string::npos)
            cutPoint = hardLimitCutPoint;
        else
            cutPoint = positionInText + segmentLength;

        AnnotationItem fragmentItem(item, text.substr(prevCutPoint, cutPoint - prevCutPoint));

        fragmentedSegmentsQueue_.push(fragmentItem);
    }
}

size_t Cutter::findSoftLimitCutPoint_(
    const std::string& text,
    size_t cutPoint,
    size_t maxCutPoint) {

    cutPoint += segmentLengthSoftLimit();

    while (cutPoint < maxCutPoint) {
        if (isSoftLimitCharacter_(text[cutPoint]))
            return cutPoint;

        ++cutPoint;
    }

    return maxCutPoint;
}

size_t Cutter::findHardLimitCutPoint_(
    const std::string& text,
    size_t cutPoint) {

    const size_t UTF8_MAX_LENGTH = 4;

    size_t hardLimitPos = cutPoint + segmentLengthHardLimit();

    size_t candidatePos =
        (hardLimitPos > UTF8_MAX_LENGTH
         ? hardLimitPos - UTF8_MAX_LENGTH
         : 0);

    while (candidatePos <= hardLimitPos)
        candidatePos += symbolLength(text, candidatePos);

    if (candidatePos <= cutPoint)
        throw Exception("segment hard limit is so small that a Unicode character cannot be contained in it, make hard limit larger");

    return candidatePos;
}

bool Cutter::isSoftLimitCharacter_(char c) {
    return c == ' ';
}
