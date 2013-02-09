#ifndef HFST_ENGLISH_TAGS_PARSER_HDR
#define HFST_ENGLISH_TAGS_PARSER_HDR

#include <string>
#include <vector>
#include <map>
#include <boost/program_options/variables_map.hpp>
#include "hfst_tags_parser.hpp"
#include "hfst_english_stemmer.hpp"

class HfstEnglishTagsParser : public HfstTagsParser {
    friend class HfstEnglishStemmer;
public:
    HfstEnglishTagsParser(const boost::program_options::variables_map& options);
    virtual HfstTags getLexemeAttributes(HfstRawTags tags);
    virtual HfstTags getFormAttributes(HfstRawTags tags);
    virtual ~HfstEnglishTagsParser();
private:
    const static std::string INNER_SEPARATOR;
    static std::map<std::string, std::string> PREDEFINED_TAGS;
    static std::vector<std::string> TAGS_ALLOWED_AS_POS;

    HfstTags parse(HfstRawTags tags);

    std::string getPartOfSpeechTag_(std::map<std::string, std::string> attributes);
    std::string getAttributeName(const std::string &value);
};

#endif
