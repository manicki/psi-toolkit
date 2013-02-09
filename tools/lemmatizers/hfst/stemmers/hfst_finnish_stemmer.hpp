#ifndef HFST_FINNISH_STEMMER_HDR
#define HFST_FINNISH_STEMMER_HDR

#include <string>
#include <vector>
#include <map>
#include <boost/program_options/variables_map.hpp>
#include "hfst_adapter_interface.hpp"
#include "hfst_stemmer.hpp"

class HfstFinnishStemmer : public HfstStemmer {
    public:
        HfstFinnishStemmer(const boost::program_options::variables_map& options);
        virtual HfstStems stem(const std::string &, HfstAdapterInterface &);
        virtual ~HfstFinnishStemmer() { };
    private:
        const static std::string HFST_COMPOUND_BOUNDARY;

        bool includeCompounds_;

        bool excludeInterpretation_(const std::string &boundaryType);
};

#endif
