#include "hfst_german_tags_parser.hpp"
#include "regexp.hpp"
#include "logging.hpp"

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

const std::string HfstGermanTagsParser::INNER_SEPARATOR = "/";

std::multimap<std::string, std::string> HfstGermanTagsParser::LEXEME_TAGS =
    boost::assign::map_list_of
        ("nn", "gender")
        ("art", "definiteness");

std::set<std::string> HfstGermanTagsParser::PREDEFINED_POS_TAGS =
    boost::assign::list_of
        ("adj")
        ("adv")
        ("art")
        ("card")
        ("konj")
        ("ne")
        ("nn")
        ("ord")
        ("other")
        ("pref")
        ("prep")
        ("ptkl")
        ("v")
        ("poss")
        ("ppro")
        ("wadv")
        ("wpro")
        ("dem")
        ("rel")
        ("indef");

std::map<std::string, std::string> HfstGermanTagsParser::PREDEFINED_ATTR_TAGS =
    boost::assign::map_list_of
        ("nom", "case")
        ("gen", "case")
        ("dat", "case")
        ("akk", "case")
        ("masc", "gender")
        ("fem", "gender")
        ("neut", "gender")
        ("nogend", "gender")
        ("sg", "number")
        ("pl", "number")
        ("1", "person")
        ("2", "person")
        ("3", "person")
        ("pres", "tense")
        ("past", "tense")
        ("inf", "tense") //@todo
        ("ppast", "participle") //@todo
        ("ppres", "participle") //@todo
        ("ind", "mood")
        ("imp", "mood")
        ("konj", "mood")
        ("def", "definiteness")
        ("indef", "definiteness")
        ("pos", "degree")
        ("comp", "degree")
        ("sup", "degree")
        ("pred", "pred") //@todo
        ("st", "declension")
        ("sw", "declension")
        ("mix", "declension")
        ("pers", "pronoun_type") //@todo
        ("refl", "pronoun_type") //@todo
        ("attr", "pronoun_type") //@todo
        ("pro", "pronoun_type") //@todo
        ("kon", "conjunction_type") //@todo
        ("sub", "conjunction_type") //@todo
        ("vgl", "conjunction_type") //@todo
        ("ant", "particle_type") //@todo
        ;

std::vector<std::string> HfstGermanTagsParser::TAGS_ALLOWED_AS_POS =
    boost::assign::list_of
        ("pos");

std::vector<std::string> HfstGermanTagsParser::TAGS_TO_SKIP =
    boost::assign::list_of
        ("cap")
        ("subst")
        ("suff");


HfstGermanTagsParser::HfstGermanTagsParser(const boost::program_options::variables_map&) {
}

HfstGermanTagsParser::~HfstGermanTagsParser() {
}

std::vector<std::map<std::string, std::string> > HfstGermanTagsParser::parse(
        std::vector<std::string> tags) {
    std::vector<std::map<std::string, std::string> > attributes;
    RegExp regAttrib("<([^\\+]*)>");
    RegExp regPos("<\\+([^\\+]*)>");
    for (std::vector<std::string>::iterator tagIt = tags.begin();
            tagIt != tags.end(); ++ tagIt) {
        std::string tag = *tagIt;
        std::string tagValue = "";
        bool isPosTag = false;
        if (RegExp::FullMatch(tag, regPos, &tagValue)) {
            boost::algorithm::to_lower(tagValue);
            isPosTag = true;
        } else if (RegExp::FullMatch(tag, regAttrib, &tagValue)) {
            boost::algorithm::to_lower(tagValue);
        }
        if (tagValue != "") {
            if (skipTag_(tagValue)) {
                continue;
            }
            std::vector<std::map<std::string, std::string> > tempAttributes;
            std::vector<std::string> singleValues;
            boost::split(singleValues, tagValue, boost::is_any_of(INNER_SEPARATOR));
            if (!attributes.empty()) {
                for (std::vector<std::map<std::string, std::string> >::iterator attribIt =
                        attributes.begin(); attribIt != attributes.end(); ++attribIt) {
                    BOOST_FOREACH(std::string value, singleValues) {
                        std::string attribName = "";
                        if (isPosTag) {
                            if (isValidPosTag_(value)) {
                                attribName = "pos";
                            } else {
                                DEBUG("Unknown part of speech: " << value);
                                continue;
                            }
                        } else {
                            if (!isValidPosTag_(value))
                                attribName = getAttributeName_(value);
                            else
                                attribName = "pos";
                        }
                        if (attribName != "") {
                            std::map<std::string, std::string> attribs = *attribIt;
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
                    std::string attribName = "";
                    if (isPosTag) {
                        if (isValidPosTag_(value)) {
                            attribName = "pos";
                        } else {
                            DEBUG("Unknown part of speech: " << value);
                            continue;
                        }
                    } else {
                        if (!isValidPosTag_(value))
                            attribName = getAttributeName_(value);
                        else
                            attribName = "pos";
                    }
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
    }
    return attributes;
}

bool HfstGermanTagsParser::isValidPosTag_(const std::string &value) {
    std::set<std::string>::iterator foundPos = PREDEFINED_POS_TAGS.find(value);
    return (foundPos != PREDEFINED_POS_TAGS.end());
}

std::string HfstGermanTagsParser::getAttributeName_(const std::string &value) {
    std::map<std::string, std::string>::iterator foundAttribute =
        PREDEFINED_ATTR_TAGS.find(value);
    if (foundAttribute != PREDEFINED_ATTR_TAGS.end())
        return foundAttribute->second;
    return "";
}

std::vector<std::map<std::string, std::string> > HfstGermanTagsParser::getLexemeAttributes(
        std::vector<std::string> tags) {
    std::vector<std::map<std::string, std::string> > lexemeAttributes;
    std::vector<std::map<std::string, std::string> > allAttributes = parse(tags);
    for (std::vector<std::map<std::string, std::string> >::iterator attribIt = 
            allAttributes.begin(); attribIt != allAttributes.end(); ++ attribIt) {
        std::string pos = getPartOfSpeechTag_(*attribIt);
        if (!pos.empty()) {
            std::map<std::string, std::string> interpAttributes;
            interpAttributes.insert(std::pair<std::string, std::string>("pos", pos));
            std::pair<
                std::multimap<std::string, std::string>::iterator,
                std::multimap<std::string, std::string>::iterator
               > ltIt =
                LEXEME_TAGS.equal_range(pos);
            for (std::multimap<std::string, std::string>::iterator ltattrIt =
                    ltIt.first; ltattrIt != ltIt.second; ++ ltattrIt) {
                std::map<std::string, std::string>::iterator ltvalIt =
                    attribIt->find(ltattrIt->second);
                if (ltvalIt != attribIt->end()) {
                    std::string ltVal = ltvalIt->second;
                    interpAttributes.insert(std::make_pair(ltattrIt->second, ltVal));
                }
            }
            lexemeAttributes.push_back(interpAttributes);
        } else {
            DEBUG("No part of speech found");
        }
    }
    return lexemeAttributes;
}

std::vector<std::map<std::string, std::string> > 
HfstGermanTagsParser::getFormAttributes(std::vector<std::string> tags) {
    std::vector<std::map<std::string, std::string> > formAttributes;
    std::vector<std::map<std::string, std::string> > allAttributes = parse(tags);
    for (std::vector<std::map<std::string, std::string> >::iterator attribsIt = 
            allAttributes.begin(); attribsIt != allAttributes.end(); ++ attribsIt) {
        std::map<std::string, std::string> attributes = *attribsIt;
        std::string pos = getPartOfSpeechTag_(attributes);
        attributes["pos"] = pos;
        formAttributes.push_back(attributes);
    }
    return formAttributes;
}

std::string HfstGermanTagsParser::getPartOfSpeechTag_(
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

bool HfstGermanTagsParser::skipTag_(const std::string &value) {
    BOOST_FOREACH(std::string tag, TAGS_TO_SKIP) {
        if (tag == value)
            return true;
    }
    return false;
}
