#ifndef LEX_DB_LEMMATIZER_HDR
#define LEX_DB_LEMMATIZER_HDR

#include <string>
#include <list>

#include <pqxx/connection>

#include "annotation_item_manager.hpp"
#include "lemmatizer_output_iterator.hpp"

class LexDbLemmatizer {

public:
    LexDbLemmatizer();

    void lemmatize(const std::string token,
                   AnnotationItemManager& annotationItemManager,
                   LemmatizerOutputIterator& outputIterator);

    static std::string getName();

    std::list<std::string> getLayerTags();

private:
    pqxx::connection connection_;

    void parseSinflection_(const std::string& sinflection,
                           std::string& partOfSpeech,
                           std::string& flags);
};



#endif
