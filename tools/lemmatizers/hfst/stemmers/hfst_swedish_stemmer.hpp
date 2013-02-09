#ifndef HFST_SWEDISH_STEMMER_HDR
#define HFST_SWEDISH_STEMMER_HDR

#include <string>
#include <vector>
#include <map>
#include <boost/program_options/variables_map.hpp>
#include "hfst_adapter_interface.hpp"
#include "hfst_stemmer.hpp"

class HfstSwedishStemmer : public HfstStemmer {
    public:
        HfstSwedishStemmer(const boost::program_options::variables_map& options);
        virtual HfstStems stem(const std::string &, HfstAdapterInterface &);
        virtual ~HfstSwedishStemmer() { };
};

#endif
