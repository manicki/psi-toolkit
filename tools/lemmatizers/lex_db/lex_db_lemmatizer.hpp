#ifndef LEX_DB_LEMMATIZER_HDR
#define LEX_DB_LEMMATIZER_HDR

class LexDbLemmatizer {

public:
    LexDbLemmatizer();

    void lemmatize(const std::string token,
                   AnnotationItemManager& annotationItemManager,
                   LemmatizerOutputIterator& outputIterator);

    static std::string getName();

    std::list<std::string> getLayerTags();
};



#endif
