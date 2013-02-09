#ifndef HFST_STEMMER_HDR
#define HFST_STEMMER_HDR

#include <string>
#include <vector>
#include <map>
#include "hfst_adapter_interface.hpp"
#include "hfst_lemmatizer_types.hpp"

class HfstStemmer {
    public:
        virtual HfstStems stem(const std::string &, HfstAdapterInterface &) = 0;
        virtual ~HfstStemmer() { };
};

#endif
