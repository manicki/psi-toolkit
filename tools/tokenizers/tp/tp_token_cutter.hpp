#ifndef TP_TOKEN_CUTTER_HDR
#define TP_TOKEN_CUTTER_HDR

#include "annotation_item.hpp"
#include "cutter.hpp"

class TpTokenCutter : public Cutter {
private:
    virtual AnnotationItem doCutOff(const std::string& text, size_t& positionInText);
    virtual int doMaximumFragmentLength();
};

#endif
