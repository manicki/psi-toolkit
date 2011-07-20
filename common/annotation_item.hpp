#ifndef ANNOTATION_ITEM_HDR
#define ANNOTATION_ITEM_HDR

#include <string>
#include <vector>

class AnnotationItem {

public:

    AnnotationItem(const std::string& category);

    std::string getCategory() const;

    bool operator==(const AnnotationItem& other) const;

    friend class AnnotationItemManager;

private:

    std::string category_;
    std::vector<std::string> attrValues_;

};

#endif
