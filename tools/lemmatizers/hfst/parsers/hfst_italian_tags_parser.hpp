#ifndef HFST_ITALIAN_TAGS_PARSER_HDR
#define HFST_ITALIAN_TAGS_PARSER_HDR

#include <string>
#include <vector>
#include <map>
#include <boost/program_options/variables_map.hpp>
#include "hfst_tags_parser.hpp"

class HfstItalianTagsParser : public HfstTagsParser {
public:
    HfstItalianTagsParser(const boost::program_options::variables_map& options);
    virtual HfstTags getLexemeAttributes(HfstRawTags tags);
    virtual HfstTags getFormAttributes(HfstRawTags tags);
    virtual ~HfstItalianTagsParser();
private:
    static std::multimap<std::string, std::string> LEXEME_TAGS;
    static std::vector<std::string> COMPLEX_POS_TAG_PARTS;
    static std::vector<std::string> PREDEFINED_POS_TAGS;
    static std::map<std::string, std::string> PREDEFINED_TAGS;
    static std::vector<std::string> TAGS_ALLOWED_AS_POS;
    static std::vector<std::string> TAGS_TO_SKIP;

    HfstTags parse(HfstRawTags tags);
    std::string getPartOfSpeechTag_(std::map<std::string, std::string> attributes);

    std::string getAttributeName_(const std::string &value);
    bool skipTag_(const std::string &tag);
    bool isComplexPosTagPart_(const std::string &tag);
    bool isValidPosTag_(const std::string &tag);
};

#endif
