#include "hfst_swedish_tags_parser.hpp"
#include "regexp.hpp"
#include "logging.hpp"

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

std::multimap<std::string, std::string> HfstSwedishTagsParser::LEXEME_TAGS =
    boost::assign::map_list_of
        ("substantiv", "gender");

std::map<std::string, std::string> HfstSwedishTagsParser::PREDEFINED_TAGS =
    boost::assign::map_list_of
        ("adjektiv", "pos")
        ("adverb", "pos")
        ("deponens", "pos")
        ("egennamn", "pos")
        ("interjektion", "pos")
        ("konjunktion", "pos")
        ("prefix", "pos")
        ("preposition", "pos")
        ("pronomen", "pos")
        ("rakneord", "pos")  //numeral (both cardinal and ordinal)
        ("substantiv", "pos")
        ("verb", "pos")
        ("utrum", "gender")   //common
        ("neutrum", "gender") //neuter
        ("mask", "gender")    //only definite singular positive form of adjectives
        ("sg", "number")
        ("pl", "number")
        ("nom", "case")
        ("gen", "case")
        ("presens", "tense")
        ("preteritum", "tense")
        ("infinitiv", "tense") //@todo
        ("supinum", "tense")   //@todo
        ("participle", "presens_particip") //@todo
        ("participle", "perfekt_particip") //@todo
        ("conditional", "mood")
        ("imperativ", "mood")
        ("konjunktiv", "mood")
        ("pos", "degree")
        ("komp", "degree")
        ("sup", "degree")
        ("best", "definiteness")  //definite
        ("obest", "definiteness") //indefinite
        ("aktiv", "voice")
        ("passiv", "voice")
        ;

std::vector<std::string> HfstSwedishTagsParser::TAGS_ALLOWED_AS_POS =
    boost::assign::list_of
        ("pos");

HfstSwedishTagsParser::HfstSwedishTagsParser(const boost::program_options::variables_map&) {
}

HfstSwedishTagsParser::~HfstSwedishTagsParser() {
}

HfstTags HfstSwedishTagsParser::parse(HfstRawTags tags) {
    HfstTags attributes;
    RegExp regAttrib("<([^<>]*)>");
    std::map<std::string, std::string> attribs;
    for (HfstRawTags::iterator tagIt = tags.begin(); tagIt != tags.end(); ++ tagIt) {
        std::string tag = *tagIt;
        std::string tagValue = "";
        if (RegExp::FullMatch(tag, regAttrib, &tagValue)) {
            boost::algorithm::to_lower(tagValue);
            std::string attribName = getAttributeName_(tagValue);
            if (attribName != "") {
                attribs.insert(std::pair<std::string, std::string>(
                            attribName, tagValue));
            } else {
                DEBUG("Unrecognized attribute: " << tagValue);
            }
        }
    }
    attributes.push_back(attribs);
    return attributes;
}

std::string HfstSwedishTagsParser::getAttributeName_(const std::string &value) {
    std::map<std::string, std::string>::iterator foundAttribute =
        PREDEFINED_TAGS.find(value);
    if (foundAttribute != PREDEFINED_TAGS.end())
        return foundAttribute->second;
    return "";
}


HfstTags HfstSwedishTagsParser::getLexemeAttributes(HfstRawTags tags) {
    HfstTags lexemeAttributes;
    HfstTags allAttributes = parse(tags);
    for (HfstTags::iterator attribIt = allAttributes.begin(); 
            attribIt != allAttributes.end(); ++ attribIt) {
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

HfstTags HfstSwedishTagsParser::getFormAttributes(HfstRawTags tags) { 
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

std::string HfstSwedishTagsParser::getPartOfSpeechTag_(
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
