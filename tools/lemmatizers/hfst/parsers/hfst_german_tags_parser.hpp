#ifndef HFST_GERMAN_TAGS_PARSER_HDR
#define HFST_GERMAN_TAGS_PARSER_HDR

#include <string>
#include <vector>
#include <map>
#include <boost/program_options/variables_map.hpp>
#include "hfst_tags_parser.hpp"

class HfstGermanTagsParser : public HfstTagsParser {
public:
    HfstGermanTagsParser(const boost::program_options::variables_map& options);
    virtual std::vector<std::map<std::string, std::string> > getLexemeAttributes(std::vector<std::string> tags);
    virtual std::vector<std::map<std::string, std::string> > getFormAttributes(std::vector<std::string> tags);
    virtual ~HfstGermanTagsParser();
private:

    const static std::string INNER_SEPARATOR;
    static std::multimap<std::string, std::string> LEXEME_TAGS;
    static std::set<std::string> PREDEFINED_POS_TAGS;
    static std::map<std::string, std::string> PREDEFINED_ATTR_TAGS;
    static std::vector<std::string> TAGS_ALLOWED_AS_POS;
    static std::vector<std::string> TAGS_TO_SKIP;

    std::vector<std::map<std::string, std::string> > parse(std::vector<std::string> tags);
    std::string getPartOfSpeechTag_(std::map<std::string, std::string> attributes);

    std::string getAttributeName_(const std::string &value);
    bool isValidPosTag_(const std::string &value);

    bool skipTag_(const std::string &value);
};

#endif
