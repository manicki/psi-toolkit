#ifndef ANNOTATION_ITEM_HDR
#define ANNOTATION_ITEM_HDR

#include <string>

class AnnotationItem {

public:

    AnnotationItem(const std::string& category);

    std::string getCategory() const;

private:

    std::string category_;

};

#endif
