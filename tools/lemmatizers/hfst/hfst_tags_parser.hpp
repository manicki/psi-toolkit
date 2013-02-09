#ifndef HFST_TAGS_PARSER_HDR
#define HFST_TAGS_PARSER_HDR

#include <string>
#include <vector>
#include <map>
#include <boost/program_options/variables_map.hpp>
#include "hfst_lemmatizer_types.hpp"

class HfstTagsParser {
public:
    virtual HfstTags getLexemeAttributes(HfstRawTags tags) = 0;
    virtual HfstTags getFormAttributes(HfstRawTags tags) = 0;
    virtual ~HfstTagsParser() { }
};

#endif
