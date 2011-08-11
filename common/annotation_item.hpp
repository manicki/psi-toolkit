#ifndef ANNOTATION_ITEM_HDR
#define ANNOTATION_ITEM_HDR

#include <locale>
#include <string>
#include <vector>

#include <boost/dynamic_bitset.hpp>


class AnnotationItem {

public:

    AnnotationItem(const std::string& category) :
        category_(category),
        attributes_(32)
    { }

    AnnotationItem(const std::string & category, int size) :
        category_(category),
        attributes_(size % 32 == 0 ? size : size + 32 - size % 32)
    { }

    std::string getCategory() const;

    long getHash() const;

    bool operator==(const AnnotationItem& other) const;

    friend class AnnotationItemManager;

private:

    std::string category_;
    std::vector<std::string> values_;
    boost::dynamic_bitset<> attributes_;

    size_t resize_(size_t size);

};

#endif
