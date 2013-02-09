#ifndef HFST_SWEDISH_TAGS_PARSER_HDR
#define HFST_SWEDISH_TAGS_PARSER_HDR

#include <string>
#include <vector>
#include <map>
#include <boost/program_options/variables_map.hpp>
#include "hfst_tags_parser.hpp"

class HfstSwedishTagsParser : public HfstTagsParser {
public:
    HfstSwedishTagsParser(const boost::program_options::variables_map& options);
    virtual HfstTags getLexemeAttributes(HfstRawTags tags);
    virtual HfstTags getFormAttributes(HfstRawTags tags);
    virtual ~HfstSwedishTagsParser();
private:
    static std::multimap<std::string, std::string> LEXEME_TAGS;
    static std::map<std::string, std::string> PREDEFINED_TAGS;
    static std::vector<std::string> TAGS_ALLOWED_AS_POS;

    HfstTags parse(HfstRawTags tags);

    std::string getPartOfSpeechTag_(std::map<std::string, std::string> attributes);
    std::string getAttributeName_(const std::string &value);
};

#endif
