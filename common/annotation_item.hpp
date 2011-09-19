#ifndef ANNOTATION_ITEM_HDR
#define ANNOTATION_ITEM_HDR

#include <locale>
#include <string>
#include <vector>

#include <boost/dynamic_bitset.hpp>
#include <boost/foreach.hpp>


class AnnotationItem {

public:
    AnnotationItem(const std::string& category) :
        category_(category),
        attributes_(32)
    { }

    AnnotationItem(const std::string& category, const std::string& text) :
        category_(category),
        text_(text),
        attributes_(32)
    { }

    AnnotationItem(const std::string& category, int size) :
        category_(category),
        attributes_(size % 32 == 0 ? size : size + 32 - size % 32)
    { }

    AnnotationItem(const std::string& category, const std::string& text, int size) :
        category_(category),
        text_(text),
        attributes_(size % 32 == 0 ? size : size + 32 - size % 32)
    { }


    std::string getCategory() const;

    std::string getText() const;

    long getHash() const;

    bool operator==(const AnnotationItem& other) const;

    friend class AnnotationItemManager;

private:

    std::string category_;
    std::string text_;
    std::vector<std::string> values_;
    boost::dynamic_bitset<> attributes_;

    size_t resize_(size_t size);
    bool areAttributesTheSame_(const AnnotationItem& other) const;
};

#endif
