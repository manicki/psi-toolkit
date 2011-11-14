#ifndef BY_SPACES_CUTTER_HDR
#define BY_SPACES_CUTTER_HDR

#include "annotation_item.hpp"
#include "cutter.hpp"

class BySpacesCutter : public Cutter {
public:
    virtual ~BySpacesCutter();

private:
    virtual AnnotationItem doCutOff(const std::string& text, size_t& positionInText);
    virtual AnnotationItem doCutOff(const StringFrag text, size_t& positionInText);
    virtual int doMaximumFragmentLength();
    virtual std::list<std::string> doLayerTags();
};

#endif
