#include "morfologik_tags_parser.hpp"
#include "logging.hpp"

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <algorithm>

std::string MorfologikTagsParser::outerSeparator = ":";
std::string MorfologikTagsParser::innerSeparator = ".";

std::multimap<std::string, std::string> MorfologikTagsParser::lexemeTags =
	boost::assign::map_list_of
		("subst", "gender")
		("verb", "aspect");

std::map<std::string, std::string> MorfologikTagsParser::predefinedTags = 
	boost::assign::map_list_of 
		("adj", "pos")
		("adjp", "pos")
		("adv", "pos")
		("conj", "pos")
		("ign", "pos")
		("indecl", "pos")
		("num", "pos")
		("pact", "pos")
		("pant", "pos")
		("pcon", "pos")
		("ppas", "pos")
		("ppron12", "pos")
		("ppron3", "pos")
		("pred", "pos")
		("prep", "pos")
		("siebie", "pos")
		("subst", "pos")
		("verb", "pos")
		("sg", "number")
		("pl", "number")
		("nom", "case")
		("gen", "case")
		("acc", "case")
		("dat", "case")
		("inst", "case")
		("loc", "case")
		("voc", "case")
		("pos", "degree")
		("comp", "degree")
		("sup", "degree")
		("m", "gender")
		("m1", "gender")
		("m2", "gender")
		("m3", "gender")
		("m4", "gender")
		("n", "gender")
		("f", "gender")
		("pri", "person")
		("sec", "person")
		("ter", "person")
		("depr", "depreciativity")
		("aff", "negation")
		("neg", "negation")
		("perf", "aspect")
		("imperf", "aspect")
		("?perf", "aspect")
		("nakc", "accentability")
		("akc", "accentability")
		("praep", "post-prepositionality")
		("npraep", "post-prepositionality")
		("ger", "tense")
		("imps", "tense")
		("inf", "tense")
		("fin", "tense")
		("bedzie", "tense")
		("praet", "tense")
		("refl", "tense")
		("impt", "mode")
		("pot", "mode")
		("indecl", "uninflected")
		("qub", "uninflected")
		("irreg", "irregularity");

std::vector<std::map<std::string, std::string> > MorfologikTagsParser::getFormAttributes(
	std::string & tag
) {

	int complexAttributeCounter = 0;
	std::vector<std::map<std::string, std::string> > allDescriptions;
	std::map<std::string, std::string> rawDescription;

	rawDescription = parseSimple(tag, complexAttributeCounter); 
	removeLexemeAttributes(rawDescription);

	if (complexAttributeCounter == 0) {
		allDescriptions.insert(allDescriptions.begin(), rawDescription);
		return allDescriptions;
	}
	allDescriptions = expandDescriptions(rawDescription, complexAttributeCounter);

	return allDescriptions;	
}

void MorfologikTagsParser::removeLexemeAttributes(
	std::map<std::string, std::string> & description
) {
	std::string pos = description["pos"];

	std::multimap<std::string, std::string>::iterator it;
	for (it = lexemeTags.equal_range(pos).first; it != lexemeTags.equal_range(pos).second; it++) {
		description.erase(it->second);
	}
	description.erase("pos");
}

std::vector<std::map<std::string, std::string> > MorfologikTagsParser::parse(
	std::string & tag
) {

	int complexAttributeCounter = 0;
	std::vector<std::map<std::string, std::string> > allDescriptions;
	std::map<std::string, std::string> rawDescription;

	rawDescription = parseSimple(tag, complexAttributeCounter); 

	if (complexAttributeCounter == 0) {
		allDescriptions.insert(allDescriptions.begin(), rawDescription);
		return allDescriptions;
	}
	allDescriptions = expandDescriptions
		(rawDescription, complexAttributeCounter);
	return allDescriptions;
}

std::vector<std::map<std::string, std::string> > 
MorfologikTagsParser::expandDescriptions(
	std::map<std::string, std::string> description, 
	int & complexAttributeCounter
) {

	std::vector<std::map<std::string, std::string> > allDescriptions;
	allDescriptions.insert(allDescriptions.begin(), description);
	
	while (complexAttributeCounter > 0) {
		std::vector<std::map<std::string, std::string> > tempDescriptions;
		for (unsigned int i = 0; i < allDescriptions.size(); i++)
		{
			std::vector<std::map<std::string, std::string> > expanded = 
				expandSingleDescription(allDescriptions[i]);
			tempDescriptions.insert(
				tempDescriptions.end(), expanded.begin(), expanded.end());
		}
		allDescriptions = tempDescriptions;
		complexAttributeCounter--;
	}
	return allDescriptions;
}

std::vector<std::map<std::string, std::string> > 
MorfologikTagsParser::expandSingleDescription(
	std::map<std::string, std::string> description
) {

	int lengthOfCurrentComplexAttribute = getLengthOfFirstComplexAttribute(
		description);

	std::vector<std::map<std::string, std::string> > result;
	for (int i = 0; i < lengthOfCurrentComplexAttribute; i++) {
		std::map<std::string, std::string> m;
		result.push_back(m);
	}

	bool firstComplexAttribute = true;
	std::map<std::string, std::string>::iterator it;
	for (it = description.begin(); it != description.end() ; it++) {		
		
		size_t innerSeparatorPosition = it->second.find(innerSeparator);
		if ((firstComplexAttribute) 
			&& (innerSeparatorPosition != std::string::npos)) {

			std::vector<std::string> subAttrContainer;
			boost::split(subAttrContainer, it->second, 
				boost::is_any_of(innerSeparator));
			
			for (int i = 0; i < lengthOfCurrentComplexAttribute; i++) {
				result[i].insert( std::pair<std::string, std::string>
					(it->first, subAttrContainer[i]) );				
			}
			firstComplexAttribute = false;
		}
		else {
			for (int i = 0; i < lengthOfCurrentComplexAttribute; i++) {
				result[i].insert( std::pair<std::string, std::string>
					(it->first, it->second) );
			}			
		}
	}
	return result;
}

std::map<std::string, std::string> MorfologikTagsParser::parseSimple(
	std::string & tag, int & counter
) {

	std::map<std::string, std::string> descriptions;
	std::map<std::string, std::string>::iterator foundAttribute;

	std::vector<std::string> attributes;
	boost::split(attributes, tag, boost::is_any_of(outerSeparator));

	for (int i = 0; i < (int)attributes.size(); i++) {
		foundAttribute = predefinedTags.find(
			getAttributeToSeek(attributes[i], counter));
		
		if (foundAttribute != predefinedTags.end()) {
			descriptions.insert(std::pair<std::string, std::string>
				(foundAttribute->second, attributes[i]));
		}
		else {
			std::string message = "Attribute ";
			message += attributes[i];
			message += " has been not recognized in predefined Morfologik tags.";
			WARN(message);
		}
	}
	return descriptions;
}

std::map<std::string, std::string> MorfologikTagsParser::getLexemeAttributes(
	std::string & tag
) {
	std::map<std::string, std::string> attributes;
	int hasCmplAttr = 0;
	std::map<std::string, std::string> allAttributes = 
		parseSimple(tag, hasCmplAttr);

	// Add part of speech
	std::string pos = allAttributes["pos"];
	attributes.insert(std::pair<std::string, std::string> ("pos", pos) );
	
	// Add other attributes
	std::multimap<std::string, std::string>::iterator it;
	for (it = lexemeTags.equal_range(pos).first; 
		it != lexemeTags.equal_range(pos).second; 
		++it) {

		std::string attr = allAttributes[it->second];
		std::replace(attr.begin(), attr.end(), '.', '-');
		attributes.insert(std::make_pair(it->second, attr));
	}

	return attributes;
}

std::string MorfologikTagsParser::getAttributeToSeek(
	std::string & attribute, int & complexAttributeCounter
) {
	
	size_t innerSeparatorPosition = attribute.find(innerSeparator);
	if (innerSeparatorPosition != std::string::npos) {
		complexAttributeCounter++;
		return attribute.substr(0, int(innerSeparatorPosition));
	}
	else {
		return attribute;
	}
}

int MorfologikTagsParser::getLengthOfFirstComplexAttribute(
	std::map<std::string, std::string> tag
) {

	std::map<std::string, std::string>::iterator it;
	for (it = tag.begin(); it != tag.end() ; it++) {

		size_t innerSeparatorPosition = it->second.find(innerSeparator);
		if (innerSeparatorPosition != std::string::npos) {
			return std::count(it->second.begin(), it->second.end(), '.') + 1;
		}
	}
	return 0;
} 
