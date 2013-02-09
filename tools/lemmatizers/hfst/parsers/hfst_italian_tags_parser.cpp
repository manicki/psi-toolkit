#include "hfst_italian_tags_parser.hpp"
#include "regexp.hpp"
#include "logging.hpp"

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

std::multimap<std::string, std::string> HfstItalianTagsParser::LEXEME_TAGS =
    boost::assign::map_list_of
        ("noun", "gender")
        ("art", "gender")
        ("artpre", "gender")
        ("pro-demo", "gender")
        ("pro-demo", "number")
        ("pro-indef", "gender")
        ("pro-indef", "number")
        ("pro-pers", "person")
        ("pro-pers", "gender")
        ("pro-pers", "number")
        ("pro-pers", "clitic_pronoun")
        ("pro-poss", "gender")
        ("pro-poss", "number");

std::vector<std::string> HfstItalianTagsParser::COMPLEX_POS_TAG_PARTS =
    boost::assign::list_of
        ("demo")
        ("indef")
        ("num")
        ("card")
        ("pers")
        ("poss")
        ("wh")
        ("che"); //wh-che

std::vector<std::string> HfstItalianTagsParser::PREDEFINED_POS_TAGS =
    boost::assign::list_of
        ("abl")
        ("adj")
        ("adv")
        ("art")
        ("asp")
        ("aux")
        ("cau")
        ("ce")
        ("ci")
        ("con")
        ("det")
        ("int")
        ("mod")
        ("ne")
        ("noun")
        ("pon")
        ("pre")
        ("pro")
        ("si")
        ("tale")
        ("ver")
        ("wh");
 
std::map<std::string, std::string> HfstItalianTagsParser::PREDEFINED_TAGS =
    boost::assign::map_list_of
        ("m", "gender")
        ("f", "gender")
        ("s", "number")
        ("p", "number")
        ("pos", "degree")
        ("comp", "degree")
        ("sup", "degree")
        ("1", "person")
        ("2", "person")
        ("3", "person")
        ("cond", "mood")
        ("ger", "mood")
        ("impr", "mood")
        ("ind", "mood")
        ("inf", "mood")
        ("part", "mood")
        ("sub", "mood")
        ("pres", "tense")
        ("past", "tense")
        ("impf", "tense")
        ("fut", "tense")
        ("cela", "clitic")
        ("cele", "clitic")
        ("celi", "clitic")
        ("celo", "clitic")
        ("cene", "clitic")
        ("ci", "clitic")
        ("gli", "clitic")
        ("gliela", "clitic")
        ("gliele", "clitic")
        ("glieli", "clitic")
        ("glielo", "clitic")
        ("gliene", "clitic")
        ("la", "clitic")
        ("le", "clitic")
        ("li", "clitic")
        ("lo", "clitic")
        ("mela", "clitic")
        ("mele", "clitic")
        ("meli", "clitic")
        ("melo", "clitic")
        ("mene", "clitic")
        ("mi", "clitic")
        ("ne", "clitic")
        ("sela", "clitic")
        ("sele", "clitic")
        ("seli", "clitic")
        ("selo", "clitic")
        ("sene", "clitic")
        ("si", "clitic")
        ("tela", "clitic")
        ("tele", "clitic")
        ("teli", "clitic")
        ("telo", "clitic")
        ("tene", "clitic")
        ("ti", "clitic")
        ("vela", "clitic")
        ("vele", "clitic")
        ("veli", "clitic")
        ("velo", "clitic")
        ("vene", "clitic")
        ("vi", "clitic") 
        ("cli", "clitic_pronoun")
        ;

std::vector<std::string> HfstItalianTagsParser::TAGS_ALLOWED_AS_POS =
    boost::assign::list_of
        ("pos");

std::vector<std::string> HfstItalianTagsParser::TAGS_TO_SKIP =
    boost::assign::list_of
        ("pon")
        ("sen");


HfstItalianTagsParser::HfstItalianTagsParser(const boost::program_options::variables_map&) {
}

HfstItalianTagsParser::~HfstItalianTagsParser() {
}

HfstTags HfstItalianTagsParser::parse(HfstRawTags tags) {
    std::vector<std::map<std::string, std::string> > attributes;
    RegExp regDerivOrComplex("-([^\\-]+)");
    bool posTagSet = false;
    std::map<std::string, std::string> attribs;
    for (HfstRawTags::iterator tagIt = tags.begin();
            tagIt != tags.end(); ++ tagIt) {
        std::string tag = *tagIt;
        boost::algorithm::to_lower(tag);
        std::string tagValue = tag;
        if (RegExp::FullMatch(tag, regDerivOrComplex, &tagValue)) {
            if (isComplexPosTagPart_(tagValue)) {
                std::map<std::string, std::string>::iterator posIt =
                    attribs.find("pos");
                if (posIt != attribs.end()) {
                    posIt->second += "-" + tagValue;
                }
                continue;
            }
        }
        if (!skipTag_(tagValue)) {
            std::string attribName = "";
            if (!posTagSet && isValidPosTag_(tagValue)) {
                attribName = "pos";
                posTagSet = true;
            } else
                attribName = getAttributeName_(tagValue);
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

bool HfstItalianTagsParser::isValidPosTag_(const std::string &value) {
    BOOST_FOREACH(std::string tag, PREDEFINED_POS_TAGS) {
        if (tag == value)
            return true;
    }
    return false;
}

std::string HfstItalianTagsParser::getAttributeName_(const std::string &value) {
    std::map<std::string, std::string>::iterator foundAttribute =
        PREDEFINED_TAGS.find(value);
    if (foundAttribute != PREDEFINED_TAGS.end())
        return foundAttribute->second;
    return "";
}


bool HfstItalianTagsParser::skipTag_(const std::string &tag) {
    BOOST_FOREACH(std::string t, TAGS_TO_SKIP) {
        if (t == tag)
            return true;
    }
    return false;
}

bool HfstItalianTagsParser::isComplexPosTagPart_(const std::string &tag) {
    BOOST_FOREACH(std::string p, COMPLEX_POS_TAG_PARTS) {
        if (p == tag)
            return true;
    }
    return false;
}

HfstTags HfstItalianTagsParser::getLexemeAttributes(HfstRawTags tags) {
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

HfstTags HfstItalianTagsParser::getFormAttributes(HfstRawTags tags) {
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

std::string HfstItalianTagsParser::getPartOfSpeechTag_(
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
