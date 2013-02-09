#ifndef HFST_LEMMATIZER_TYPES_HDR
#define HFST_LEMMATIZER_TYPES_HDR

#include <string>
#include <vector>
#include <map>

typedef std::vector<std::string> HfstRawTags;
typedef std::multimap<std::string, HfstRawTags> HfstStems;
typedef std::vector<std::map<std::string, std::string> > HfstTags;

#endif
