#ifndef LEMMATIZER_HDR
#define LEMMATIZER_HDR

class Lemmatizer {
public:
    void lemmatize(const std::string& token, LemmatizerOutputIterator& lemmatizerIterator);

private:
    virtual void doLemmatize(
        const std::string& token, LemmatizerOutputIterator& lemmatizerIterator);
    
};

#endif
