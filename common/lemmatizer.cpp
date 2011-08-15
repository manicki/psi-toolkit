#include "lemmatizer.hpp"

void Lemmatizer::lemmatize(
    const std::string& token, LemmatizerOutputIterator& lemmatizerIterator) {
    doLemmatize(token, lemmatizerIterator);
}
