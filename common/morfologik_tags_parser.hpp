#ifndef MORFOLOGIK_TAGS_PARSER_HDR
#define MORFOLOGIK_TAGS_PARSER_HDR

#include <string>
#include <map>
#include <vector>

class MorfologikTagsParser {

public:

    /**
     * Finds attributes specify to the determined part of speech.
     * @param tag
     *  A string containing a Morfologik tag.
     * @return
     *  A map object with tag-attribute pairs.
     */
    std::map<std::string, std::string> getLexemeAttributes(std::string& tag);

    /**
     * Finds attributes that are not associated with parts of speech recognized
     * in input tag.
     * @param tag
     *  A string containing a Morfologik tag.
     * @return
     *  A map object with tag-attribute pairs.
     */
    std::vector<std::map<std::string, std::string> > getFormAttributes(std::string& tag);

    /**
     * Complex parses Morfologik tags.
     * @param tag
     *  A string containing a Morfologik tag.
     * @return
     *  A vector of tag-attribute pairs in map object.
     */
    std::vector<std::map<std::string, std::string> > parse(std::string& tag);

    /**
     * Simple parses Morfologik tags without splitting the complex attributes.
     * @param tag
     *  A string containing a Morfologik tag.
     * @param counter
     *  This parameter is increment for each found complex attribute.
     * @return
     *  Map object with tag-attribute pairs. Complex tags are not splitted.
     */
    std::map<std::string, std::string> parseSimple(std::string& tag, int& counter);

private:

    const static std::string OUTER_SEPARATOR;
    const static std::string INNER_SEPARATOR;

    static std::map<std::string, std::string> PREDEFINED_TAGS;
    static std::multimap<std::string, std::string> LEXEME_TAGS;
    static std::vector<std::string> TAGS_ALLOWED_AS_POS;

    void removeLexemeAttributes_(std::map<std::string, std::string>&);
    std::string getPartOfSpeechTag_(std::map<std::string, std::string>);

    std::vector<std::map<std::string, std::string> >
        expandDescriptions_(std::map<std::string, std::string>, int&);
    std::vector<std::map<std::string, std::string> >
        expandSingleDescription_(std::map<std::string, std::string>);

    std::string getAttributeToSeek_(std::string& tags, int&);
    int getLengthOfFirstComplexAttribute_(std::map<std::string, std::string> tag);
};

#endif
