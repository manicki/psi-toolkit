#ifndef HFST_FINNISH_TAGS_PARSER_HDR
#define HFST_FINNISH_TAGS_PARSER_HDR

#include <string>
#include <vector>
#include <map>
#include <boost/program_options/variables_map.hpp>
#include "hfst_tags_parser.hpp"

class HfstFinnishTagsParser : public HfstTagsParser {
public:
    HfstFinnishTagsParser(const boost::program_options::variables_map& options);
    virtual HfstTags getLexemeAttributes(HfstRawTags tags);
    virtual HfstTags getFormAttributes(HfstRawTags tags);
    virtual ~HfstFinnishTagsParser();
private:
    bool splitMergedAttributes_;
    //@todo: split-pos-tags temporarily disabled as did not work as intended
    //bool splitPosTags_;
    bool includeEncliticParticles_;
    bool includeInflectionTypes_;

    const static std::string INNER_SEPARATOR;
    static std::multimap<std::string, std::string> PREDEFINED_TAGS;
    static std::vector<std::string> TAGS_ALLOWED_AS_POS;
    //@todo: split-pos-tags temporarily disabled as did not work as intended
    //static std::map<std::string, std::map<std::string, std::string> > COMPLEX_POS_SPLIT;
    static std::vector<std::string> INTERPRETATIONS_TO_SKIP_MARKS;
    static std::multimap<std::string, std::string> ATTRIBUTE_VALUES_TO_SKIP;
    static std::vector<std::string> ATTRIBUTES_TO_SKIP;
    const static std::string ENCLITIC_PARTICLE_TAG;
    static std::vector<std::string> ENCLITIC_PARTICLE_VALUES;
    static std::vector<std::string> ATTRIBUTES_INFLECTION_TYPES;
    static std::vector<std::string> MERGED_ATTRIBUTES;
    static std::map<std::string, std::map<std::string, std::string> > MERGED_ATTRIBUTE_VALUES;

    HfstTags parse(HfstRawTags tags);
    std::string getPartOfSpeechTag_(std::map<std::string, std::string> attributes);
    std::string getActualPosTag_(const std::string &tag, 
            std::map<std::string, std::string> attributes);
    std::map<std::string, std::string> getAttributeSpecification_(const std::string &key, 
            const std::string &value);

    bool skipAttribute_(const std::string &attribute);
    bool skipAttributeValue_(const std::string &attribute, const std::string &value);
    bool isValidAttribute_(const std::string &attribute);
    bool isValidAttributeValue_(const std::string &attribute, const std::string &value);

    void addAttribute_(std::map<std::string, std::string> &attribs, 
            const std::string &attribute, const std::string &value);

};

#endif
