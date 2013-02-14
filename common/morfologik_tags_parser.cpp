#include "morfologik_tags_parser.hpp"
#include "logging.hpp"

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <algorithm>

const std::string MorfologikTagsParser::OUTER_SEPARATOR = ":";
const std::string MorfologikTagsParser::INNER_SEPARATOR = ".";

std::multimap<std::string, std::string> MorfologikTagsParser::LEXEME_TAGS =
    boost::assign::map_list_of
        ("subst", "gender")
        ("verb", "aspect");

std::map<std::string, std::string> MorfologikTagsParser::PREDEFINED_TAGS =
    boost::assign::map_list_of
        ("adj", "pos")
        ("adja", "pos")
        ("adjp", "pos")
        ("adv", "pos")
        ("num", "pos")
        ("ppron12", "pos")
        ("ppron3", "pos")
        ("pred", "pos")
        ("prep", "pos")
        ("siebie", "pos")
        ("subst", "pos")
        ("verb", "pos")
        ("conj", "pos")
        ("qub", "pos")
        ("burk", "pos")     //bound word
        ("interj", "pos")   //interjection
        ("interp", "pos")   //interpunction
        ("xxx", "pos")      //alien
        ("brev", "pos")     //abbreviation
        ("nie", "pos")
        ("ign", "pos")
        ("sg", "number")
        ("pl", "number")
        ("pltant", "number")
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
        ("n1", "gender")
        ("n2", "gender")
        ("p1", "gender")
        ("p2", "gender")
        ("p3", "gender")
        ("pri", "person")
        ("sec", "person")
        ("ter", "person")
        ("depr", "depreciativity")
        ("winien", "winien")
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
        ("pact", "tense")
        ("pant", "tense")
        ("pcon", "tense")
        ("ppas", "tense")
        ("impt", "mode")
        ("pot", "mode")
        ("indecl", "uninflected")
        ("irreg", "irregularity")
        ("pun", "fullstoppedness")
        ("npun", "fullstoppedness")
        ("wok", "vocalicity")
        ("nwok", "vocalicity")
        ("agl", "agglutination")
        ("nagl", "agglutination")
        ("_", "unknown")
        ("congr", "unknown")
        ("rec", "unknown");

std::vector<std::string> MorfologikTagsParser::TAGS_ALLOWED_AS_POS =
    boost::assign::list_of
        ("pos")
        ("depreciativity")
        ("tense")
        ("mode");

std::map<std::string, std::string> MorfologikTagsParser::POS_GENERALIZATIONS =
    boost::assign::map_list_of
        ("pact", "verb")
        ("pant", "verb")
        ("pcon", "verb")
        ("ppas", "verb");

std::vector<std::map<std::string, std::string> > MorfologikTagsParser::getFormAttributes(
    std::string & tag) {

    int complexAttrCounter = 0;
    std::vector<std::map<std::string, std::string> > allDescriptions;
    std::map<std::string, std::string> rawDescription;

    rawDescription = parseSimple(tag, complexAttrCounter);

    removeLexemeAttributes_(rawDescription, complexAttrCounter);
    DEBUG("the number of complex attributes for [" << tag << "]: " << complexAttrCounter);

    if (complexAttrCounter == 0) {
        allDescriptions.insert(allDescriptions.begin(), rawDescription);
        return allDescriptions;
    }

    return expandDescriptions_(rawDescription, complexAttrCounter);
}

void MorfologikTagsParser::removeLexemeAttributes_(std::map<std::string, std::string>& description,
    int& complexAttributesCounter) {

    std::string pos = description["pos"];

    std::multimap<std::string, std::string>::iterator it;
    for (it = LEXEME_TAGS.equal_range(pos).first; it != LEXEME_TAGS.equal_range(pos).second; ++it) {
        if (description[it->second].find(INNER_SEPARATOR) != std::string::npos)
            complexAttributesCounter--;
        description.erase(it->second);
    }
    description.erase("pos");
}

std::vector<std::map<std::string, std::string> > MorfologikTagsParser::parse(std::string& tag) {
    int complexAttrCounter = 0;
    std::vector<std::map<std::string, std::string> > allDescriptions;
    std::map<std::string, std::string> rawDescription;

    rawDescription = parseSimple(tag, complexAttrCounter);

    if (complexAttrCounter == 0) {
        allDescriptions.insert(allDescriptions.begin(), rawDescription);
        return allDescriptions;
    }

    return expandDescriptions_(rawDescription, complexAttrCounter);
}

std::vector<std::map<std::string, std::string> > MorfologikTagsParser::expandDescriptions_(
    std::map<std::string, std::string> description, int& complexAttrCounter) {

    std::vector<std::map<std::string, std::string> > allDescriptions;
    allDescriptions.insert(allDescriptions.begin(), description);

    while (complexAttrCounter > 0) {
        std::vector<std::map<std::string, std::string> > tempDescriptions;
        for (unsigned int i = 0; i < allDescriptions.size(); ++i)
        {
            std::vector<std::map<std::string, std::string> > expanded =
                expandSingleDescription_(allDescriptions[i]);
            tempDescriptions.insert(tempDescriptions.end(), expanded.begin(), expanded.end());
        }
        allDescriptions = tempDescriptions;
        complexAttrCounter--;
    }

    return allDescriptions;
}

std::vector<std::map<std::string, std::string> > MorfologikTagsParser::expandSingleDescription_(
    std::map<std::string, std::string> description) {

    int lengthOfCurrentComplexAttribute = getLengthOfFirstComplexAttribute_(description);

    std::vector<std::map<std::string, std::string> > result;
    for (int i = 0; i < lengthOfCurrentComplexAttribute; ++i) {
        std::map<std::string, std::string> m;
        result.push_back(m);
    }

    bool firstComplexAttribute = true;
    std::map<std::string, std::string>::iterator it;
    for (it = description.begin(); it != description.end() ; ++it) {

        size_t INNER_SEPARATORPosition = it->second.find(INNER_SEPARATOR);
        if ((firstComplexAttribute) && (INNER_SEPARATORPosition != std::string::npos)) {

            std::vector<std::string> subAttrContainer;
            boost::split(subAttrContainer, it->second, boost::is_any_of(INNER_SEPARATOR));

            for (int i = 0; i < lengthOfCurrentComplexAttribute; ++i) {
                result[i].insert( std::pair<std::string, std::string>
                    (it->first, subAttrContainer[i]) );
            }
            firstComplexAttribute = false;
        }
        else {
            for (int i = 0; i < lengthOfCurrentComplexAttribute; ++i) {
                result[i].insert( std::pair<std::string, std::string> (it->first, it->second) );
            }
        }
    }
    return result;
}

std::map<std::string, std::string> MorfologikTagsParser::parseSimple(std::string& tag,
    int& counter) {

    std::map<std::string, std::string> descriptions;
    std::map<std::string, std::string>::iterator foundAttribute;

    std::vector<std::string> attributes;
    boost::split(attributes, tag, boost::is_any_of(OUTER_SEPARATOR));

    for (unsigned int i = 0; i < attributes.size(); ++i) {
        foundAttribute = PREDEFINED_TAGS.find(getAttributeToSeek_(attributes[i], counter));

        if (foundAttribute != PREDEFINED_TAGS.end()) {
            descriptions.insert(std::pair<std::string, std::string>
                (foundAttribute->second, attributes[i]));
        }
        else {
            WARN("Attribute [" << attributes[i] << "] for tag [" << tag
                << "] has been not recognized as predefined Morfologik tags.");
        }
    }

    return descriptions;
}

std::map<std::string, std::string> MorfologikTagsParser::getLexemeAttributes(std::string & tag) {
    std::map<std::string, std::string> attributes;
    int hasCmplAttr = 0;
    std::map<std::string, std::string> allAttributes = parseSimple(tag, hasCmplAttr);

    // Add part of speech
    std::string pos = getPartOfSpeechTag_(allAttributes);
    if (pos.empty()) {
        WARN("Part of speech for tag [" << tag << "] has been not found.");
    }
    attributes.insert(std::pair<std::string, std::string> ("pos", pos) );

    // Add other attributes
    std::multimap<std::string, std::string>::iterator it;
    for (it = LEXEME_TAGS.equal_range(pos).first; it != LEXEME_TAGS.equal_range(pos).second; ++it) {
        std::string attr = allAttributes[it->second];
        std::replace(attr.begin(), attr.end(), '.', '-');
        attributes.insert(std::make_pair(it->second, attr));
    }

    return attributes;
}

std::string MorfologikTagsParser::getPartOfSpeechTag_(
    std::map<std::string, std::string> attributes) {
    std::string selectedPos = "";

    BOOST_FOREACH(std::string pos, TAGS_ALLOWED_AS_POS) {
        if (!attributes[pos.c_str()].empty()) {
            selectedPos = attributes[pos];
            break;
        }
    }
    if (POS_GENERALIZATIONS.find(selectedPos) != POS_GENERALIZATIONS.end()) {
        selectedPos = POS_GENERALIZATIONS[selectedPos];
    }

    return selectedPos;
}

std::string MorfologikTagsParser::getAttributeToSeek_(std::string& attribute,
    int& complexAttrCounter) {

    size_t INNER_SEPARATORPosition = attribute.find(INNER_SEPARATOR);
    if (INNER_SEPARATORPosition != std::string::npos) {
        ++complexAttrCounter;
        return attribute.substr(0, int(INNER_SEPARATORPosition));
    }

    return attribute;
}

int MorfologikTagsParser::getLengthOfFirstComplexAttribute_(
    std::map<std::string, std::string> tag) {

    std::map<std::string, std::string>::iterator it;

    for (it = tag.begin(); it != tag.end() ; ++it) {
        size_t INNER_SEPARATORPosition = it->second.find(INNER_SEPARATOR);

        if (INNER_SEPARATORPosition != std::string::npos) {
            return std::count(it->second.begin(), it->second.end(), '.') + 1;
        }
    }

    return 0;
}
