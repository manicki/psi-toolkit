#ifndef CUTTER_HDR
#define CUTTER_HDR

#include "annotation_item.hpp"

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
     * Maximum fragment (in bytes) to be cut off by the cutter.
     */
    int maximumFragmentLength();

private:
    virtual AnnotationItem doCutOff(const std::string& text, size_t& positionInText) = 0;
    virtual int doMaximumFragmentLength() = 0;
};


#endif
