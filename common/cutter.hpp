#ifndef CUTTER_HDR
#define CUTTER_HDR

#include <list>
#include <string>
#include <queue>

#include "annotation_item.hpp"
#include "string_frag.hpp"
#include "psi_exception.hpp"

/**
 * Base class for classes that cut off fragments of texts (e.g. tokenizers
 * and segmenters.
 *
 * Note that there is no mention of Lattice here, a cutter operates
 * simply on text.
 */
class Cutter {

public:

    /**
     * `text` is an UTF8-encoded string. A cutter advances the `positionInText`
     * by the length of the cut-off fragment.
     */
    AnnotationItem cutOff(const std::string& text, size_t& positionInText);

    /**
     * `sf` is a string frag. A cutter advances the `positionInText`
     * by the length of the cut-off fragment.
     */
    AnnotationItem cutOff(const StringFrag& text, size_t& positionInText);

    /**
     * Maximum fragment (in bytes) to be cut off by the cutter.
     */
    int maximumFragmentLength();

    int segmentLengthSoftLimit();

    std::list<std::string> layerTags();

    virtual ~Cutter();

    class Exception : public PsiException {
    public:
        Exception(const std::string& msg): PsiException(msg) {
        }

        virtual ~Exception() throw() {}
    };

private:
    virtual AnnotationItem doCutOff(const std::string& text, size_t& positionInText) = 0;
    virtual AnnotationItem doCutOff(const StringFrag& text, size_t& positionInText) = 0;
    virtual int doMaximumFragmentLength() = 0;
    virtual int doSegmentLengthSoftLimit() = 0;
    virtual std::list<std::string> doLayerTags() = 0;

    bool shouldFragmentQueueBeUsed_();
    AnnotationItem getFirstItemInFragmentQueue_(const std::string& text, size_t& positionInText);
    bool areLimitsBroken_(size_t segmentLength);
    bool isSoftLimitBroken_(size_t segmentLength);
    bool isSoftLimitSet_();
    bool isHardLimitBroken_(size_t segmentLength);
    bool isHardLimitSet_();

    void fragmentSegment_(
        const AnnotationItem& item,
        const std::string& text,
        size_t positionInText,
        size_t segmentLength);

    size_t findSoftLimitCutPoint_(
        const std::string& text,
        size_t cutPoint,
        size_t maxCutPoint);

    size_t findHardLimitCutPoint_(
        const std::string& text,
        size_t cutPoint);

    bool isSoftLimitCharacter_(char c);

    std::queue<AnnotationItem> fragmentedSegmentsQueue_;
};


#endif
