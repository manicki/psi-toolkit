#include "hfst_french_tags_parser.hpp"
#include "regexp.hpp"
#include "logging.hpp"

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

std::multimap<std::string, std::string> HfstFrenchTagsParser::LEXEME_TAGS =
    boost::assign::map_list_of
        ("commonNoun", "gender");

std::map<std::string, std::string> HfstFrenchTagsParser::PREDEFINED_TAGS =
    boost::assign::map_list_of
        ("adjective", "pos")
        ("commonNoun", "pos")
        ("functionWord", "pos")
        ("verb", "pos")
        ("masculine", "gender")
        ("feminine", "gender")
        ("singular", "number")
        ("plural", "number")
        ("firstPerson", "person")
        ("secondPerson", "person")
        ("thirdPerson", "person")
        ("future", "tense")
        ("imperfect", "tense")
        ("present", "tense")
        ("simplePast", "tense")
        ("past", "tense")
        ("infinitive", "tense") //@todo
        ("participle", "participle") //@todo
        ("conditional", "mood")
        ("imperative", "mood")
        ("indicative", "mood")
        ("subjunctive", "mood")
        ;

std::vector<std::string> HfstFrenchTagsParser::TAGS_ALLOWED_AS_POS =
    boost::assign::list_of
        ("pos");

HfstFrenchTagsParser::HfstFrenchTagsParser(const boost::program_options::variables_map&) {
}

HfstFrenchTagsParser::~HfstFrenchTagsParser() {
}

HfstTags HfstFrenchTagsParser::parse(HfstRawTags tags) {
    HfstTags attributes;
    RegExp regAttrib("\\+([^\\+]*)");
    std::map<std::string, std::string> attribs;
    for (HfstRawTags::iterator tagIt = tags.begin();
            tagIt != tags.end(); ++ tagIt) {
        std::string tag = *tagIt;
        std::string tagValue = "";
        if (RegExp::FullMatch(tag, regAttrib, &tagValue)) {
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

std::string HfstFrenchTagsParser::getAttributeName_(const std::string &value) {
    std::map<std::string, std::string>::iterator foundAttribute =
        PREDEFINED_TAGS.find(value);
    if (foundAttribute != PREDEFINED_TAGS.end())
        return foundAttribute->second;
    return "";
}

HfstTags HfstFrenchTagsParser::getLexemeAttributes(HfstRawTags tags) {
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

HfstTags HfstFrenchTagsParser::getFormAttributes(HfstRawTags tags) {
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

std::string HfstFrenchTagsParser::getPartOfSpeechTag_(
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
