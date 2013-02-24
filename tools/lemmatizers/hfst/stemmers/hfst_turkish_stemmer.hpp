#ifndef HFST_TURKISH_STEMMER_HDR
#define HFST_TURKISH_STEMMER_HDR

#include <string>
#include <vector>
#include <map>
#include <boost/program_options/variables_map.hpp>
#include "hfst_adapter_interface.hpp"
#include "hfst_stemmer.hpp"

class HfstTurkishStemmer : public HfstStemmer {
    public:
        HfstTurkishStemmer(const boost::program_options::variables_map& options);
        virtual HfstStems stem(const std::string &, HfstAdapterInterface &);
        virtual ~HfstTurkishStemmer() { };
};

#endif
