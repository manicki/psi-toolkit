#ifndef ANNOTATION_ITEM_HDR
#define ANNOTATION_ITEM_HDR

#include <locale>
#include <string>
#include <vector>

#include <boost/dynamic_bitset.hpp>
#include <boost/foreach.hpp>

#include "string_frag.hpp"

class AnnotationItem {

public:
    AnnotationItem(const std::string& category) :
        category_(category),
        attributes_(32)
    { }

    AnnotationItem(const std::string& category, StringFrag text) :
        category_(category),
        text_(text),
        attributes_(32)
    { }

    AnnotationItem(const std::string& category, int size) :
        category_(category),
        attributes_(size % 32 == 0 ? size : size + 32 - size % 32)
    { }

    AnnotationItem(const std::string& category, StringFrag text, int size) :
        category_(category),
        text_(text),
        attributes_(size % 32 == 0 ? size : size + 32 - size % 32)
    { }

    AnnotationItem(const AnnotationItem& item, const std::string& newText) :
        category_(item.category_),
        text_(newText),
        values_(item.values_),
        attributes_(item.attributes_) {
    }

    AnnotationItem(const AnnotationItem& item) :
        category_(item.category_),
        text_(item.text_),
        values_(item.values_),
        attributes_(item.attributes_) {
    }

    std::string getCategory() const;

    std::string getText() const;

    StringFrag getTextAsStringFrag() const;

    long getHash() const;

    bool operator==(const AnnotationItem& other) const;

    friend class AnnotationItemManager;

private:

    /**
     * Stores the annotation item's category.
     */
    std::string category_;

    /**
     * Stores the annotation item's text as a string frag.
     */
    StringFrag text_;

    /**
     * Stores the values of the annotation item's attributes.
     * The value of the n'th attribute is stored in the n'th cell of the vector.
     * Other cells are empty.
     */
    std::vector<std::string> values_;

    /**
     * The value of attributes_[n] indicates whether the value of the n'th attribute
     * is present in the values_ vector.
     */
    boost::dynamic_bitset<> attributes_;

    size_t resize_(size_t size);
    bool areAttributesTheSame_(const AnnotationItem& other) const;
};

#endif
