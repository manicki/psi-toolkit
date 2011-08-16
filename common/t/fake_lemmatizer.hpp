#ifndef FAKE_LEMMATIZER_HDR
#define FAKE_LEMMATIZER_HDR

#include <string>

#include "annotation_item_manager.hpp"
#include "lemmatizer_output_iterator.hpp"

class FakeLemmatizer {

public:
    FakeLemmatizer();

    void lemmatize(const std::string token,
                   AnnotationItemManager& annotationItemManager,
                   LemmatizerOutputIterator& outputIterator);

    static std::string getName();

};

#endif
