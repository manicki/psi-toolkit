#include "hfst_english_tags_parser.hpp"
#include "regexp.hpp"
#include "logging.hpp"

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

const std::string HfstEnglishTagsParser::INNER_SEPARATOR = "/";

std::map<std::string, std::string> HfstEnglishTagsParser::PREDEFINED_TAGS =
    boost::assign::map_list_of
        ("adj", "pos")
        ("adv", "pos")
        ("n", "pos")
        ("v", "pos")
        ("past", "tense")
        ("pres", "tense")
        ("inf", "tense")
        ("prog", "tense")
        ("ppart", "tense")
        ("pl", "number")
        ("comp", "degree")
        ("super", "degree")
        ("3sg", "person") //@todo
        ;

std::vector<std::string> HfstEnglishTagsParser::TAGS_ALLOWED_AS_POS =
    boost::assign::list_of
        ("pos");

HfstEnglishTagsParser::HfstEnglishTagsParser(const boost::program_options::variables_map&) {
}

HfstEnglishTagsParser::~HfstEnglishTagsParser() {
}

HfstTags HfstEnglishTagsParser::parse(HfstRawTags tags) {
    HfstTags attributes;
    for (HfstRawTags::iterator tagIt = tags.begin();
            tagIt != tags.end(); ++ tagIt) {
        std::string tag = *tagIt;
        boost::algorithm::to_lower(tag);
        HfstTags tempAttributes;
        std::vector<std::string> singleValues;
        boost::split(singleValues, tag, boost::is_any_of(INNER_SEPARATOR));
        if (!attributes.empty()) {
            for (std::vector<std::map<std::string, std::string> >::iterator attribIt =
                    attributes.begin(); attribIt != attributes.end(); ++attribIt) {
                BOOST_FOREACH(std::string value, singleValues) {
                    std::string attribName = getAttributeName(value);
                    if (attribName != "") {
                        std::map<std::string, std::string> attribs = *attribIt;
                        if (attribName == "pos") {
                            std::map<std::string, std::string>::iterator posIt =
                                attribs.find("pos");
                            if (posIt != attribs.end()) {
                                if (posIt->second == value) //skip derivative interpretations that have been disambiguated, e.g. containing tags [N/ADJ] and +ADJ -> should by only ADJ
                                    tempAttributes.push_back(attribs);
                            }
                            continue;
                        }
                        attribs.insert(std::pair<std::string, std::string>(
                                    attribName, value));
                        tempAttributes.push_back(attribs);
                    } else {
                        DEBUG("Unrecognized attribute: " << value);
                    }
                }
            }
        } else {
            BOOST_FOREACH(std::string value, singleValues) {
                std::string attribName = getAttributeName(value);
                if (attribName != "") {
                    std::map<std::string, std::string> attribs;
                    attribs.insert(std::pair<std::string, std::string>(attribName, value));
                    tempAttributes.push_back(attribs);
                } else
                    DEBUG("Unrecognized attribute: " << value);
            }
        }
        attributes = tempAttributes;
    }
    return attributes;
}

std::string HfstEnglishTagsParser::getAttributeName(const std::string &value) {
    std::map<std::string, std::string>::iterator foundAttribute =
        PREDEFINED_TAGS.find(value);
    if (foundAttribute != PREDEFINED_TAGS.end())
        return foundAttribute->second;
    return "";
}

HfstTags HfstEnglishTagsParser::getLexemeAttributes(HfstRawTags tags) {
    HfstTags lexemeAttributes;
    HfstTags allAttributes = parse(tags);
    for (HfstTags::iterator attribIt = allAttributes.begin(); 
            attribIt != allAttributes.end(); ++ attribIt) {
        std::string pos = getPartOfSpeechTag_(*attribIt);
        if (!pos.empty()) {
            std::map<std::string, std::string> interpAttributes;
            interpAttributes.insert(std::pair<std::string, std::string>("pos", pos));
            lexemeAttributes.push_back(interpAttributes);
        } else {
            DEBUG("No part of speech found");
        }
    }
    return lexemeAttributes;
}

HfstTags HfstEnglishTagsParser::getFormAttributes(HfstRawTags tags) {
    HfstTags formAttributes;
    HfstTags allAttributes = parse(tags);
    for (HfstTags::iterator attribsIt = allAttributes.begin(); 
            attribsIt != allAttributes.end(); ++ attribsIt) {
        std::map<std::string, std::string> attributes = *attribsIt;
        std::string pos = getPartOfSpeechTag_(attributes);
        attributes["pos"] = pos;
        formAttributes.push_back(attributes);
    }
    return formAttributes;
}

std::string HfstEnglishTagsParser::getPartOfSpeechTag_(
        std::map<std::string, std::string> attributes) {
    std::string posTag = "";
    BOOST_FOREACH(std::string pos, TAGS_ALLOWED_AS_POS) {
        if (!attributes[pos].empty()) {
            posTag = attributes[pos];
            break;
        }
    }
    return posTag;
}
