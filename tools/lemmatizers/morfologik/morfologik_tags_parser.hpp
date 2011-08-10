#ifndef MORFOLOGIK_TAGS_PARSER_HDR
#define MORFOLOGIK_TAGS_PARSER_HDR

#include <map>
#include <vector>

class MorfologikTagsParser {

public:
	std::vector<std::map<std::string, std::string> > parse(std::string & tag);

private:
	static std::string outerSeparator;
	static std::string innerSeparator;
	static std::map<std::string, std::string> predefinedTags;

	std::map<std::string, std::string> parseSimple (std::string & tag, int &);

	std::vector<std::map<std::string, std::string> > 
		expandDescriptions (std::map<std::string, std::string>, int &);
	std::vector<std::map<std::string, std::string> > 
		expandSingleDescription (std::map<std::string, std::string>);

	std::string getAttributeToSeek (std::string & tags, int &);
	int getLengthOfFirstComplexAttribute (std::map<std::string, 
										  std::string> tag);
};

#endif
