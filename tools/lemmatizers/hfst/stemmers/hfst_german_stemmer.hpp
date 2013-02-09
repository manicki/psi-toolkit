#ifndef HFST_GERMAN_STEMMER_HDR
#define HFST_GERMAN_STEMMER_HDR

#include <string>
#include <vector>
#include <map>
#include <boost/program_options/variables_map.hpp>
#include "hfst_adapter_interface.hpp"
#include "hfst_stemmer.hpp"

class HfstGermanStemmer : public HfstStemmer {
    public:
        HfstGermanStemmer(const boost::program_options::variables_map& options);
        virtual HfstStems stem(const std::string &, HfstAdapterInterface &);
        virtual ~HfstGermanStemmer() { };
};

#endif
