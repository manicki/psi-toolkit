#ifndef SFST_TAGS_PARSER_HDR
#define SFST_TAGS_PARSER_HDR

#include <string>
#include <map>
#include <vector>

class SfstTagsParser {

public:

    /**
     * Finds attributes specify to the determined part of speech.
     * @param tag
     *  A string containing a Morfologik tag.
     * @return
     *  A map object with tag-attribute pairs.
     */
    std::map<std::string, std::string> getLexemeAttributes(std::string & tag);

    /**
     * Finds attributes that are not associated with parts of speech recognized
     * in input tag.
     * @param tag
     *  A string containing a Morfologik tag.
     * @return
     *  A map object with tag-attribute pairs.
     */
    std::vector<std::map<std::string, std::string> > getFormAttributes(
        std::string & tag
        );

    /**
     * Complex parses Morfologik tags.
     * @param tag
     *  A string containing a Morfologik tag.
     * @return
     *  A vector of tag-attribute pairs in map object.
     */
    std::vector<std::map<std::string, std::string> > parse(std::string & tag);

    /**
     * Simple parses Morfologik tags without splitting the complex attributes.
     * @param tag
     *  A string containing a Morfologik tag.
     * @param counter
     *  This parameter is increment for each found complex attribute.
     * @return
     *  Map object with tag-attribute pairs. Complex tags are not splitted.
     */
    std::map<std::string, std::string> parseSimple(
        std::string & tag, int & counter
        );

private:

    static std::string outerSeparator;
    static std::string innerSeparator;

    static std::map<std::string, std::string> predefinedTags;
    static std::multimap<std::string, std::string> lexemeTags;

    void removeLexemeAttributes(std::map<std::string, std::string> &);

    std::vector<std::map<std::string, std::string> >
    expandDescriptions (std::map<std::string, std::string>, int &);
    std::vector<std::map<std::string, std::string> >
    expandSingleDescription (std::map<std::string, std::string>);

    std::string getAttributeToSeek (std::string & tags, int &);
    int getLengthOfFirstComplexAttribute (std::map<std::string,
                                                   std::string> tag);
};

#endif
