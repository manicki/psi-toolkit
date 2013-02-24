#include "hfst_turkish_tags_parser.hpp"
#include "regexp.hpp"
#include "logging.hpp"

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

std::map<std::string, std::string> HfstTurkishTagsParser::PREDEFINED_TAGS =
    boost::assign::map_list_of
    //Note: most of tags are taken from: http://www.let.rug.nl/coltekin/trmorph/
        ("adj", "pos")
        ("adv", "pos")
        ("cnj", "pos")
        ("exist", "pos")
        ("ij", "pos")
        ("n", "pos")
        ("nexist", "pos")
        ("not", "pos")
        ("np", "pos")
        ("name", "pos")
        ("pnct", "pos")
        ("postp", "pos")
        ("prn", "pos")
        ("v", "pos")
        ("vaux", "pos")
        ("neg", "neg")
        ("dem", "pronoun_type") //@todo
        ("locp", "pronoun_type") //@todo
        ("pers", "pronoun_type") //@todo
        ("pl", "number")
        ("abl", "case")
        ("dat", "case")
        ("gen", "case")
        ("ins", "case")
        ("loc", "case")
        ("p1s", "possessive") 
        ("p1p", "possessive") 
        ("p2s", "possessive") 
        ("p2p", "possessive") 
        ("p3s", "possessive") 
        ("p3p", "possessive") 
        ("ki", "ki")
        ("abil", "verb_modif")
        ("adur", "verb_modif")
        ("agel", "verb_modif")
        ("agor", "verb_modif")
        ("akal", "verb_modif")
        ("akoy", "verb_modif")
        ("ayaz", "verb_modif")
        ("iver", "verb_modif")
        ("cpl_di", "copula")
        ("cpl_mis", "copula")
        ("cpl_sa", "copula")
        ("dir", "copula")
        ("1s", "person")
        ("1p", "person")
        ("2s", "person")
        ("2p", "person")
        ("3s", "person")
        ("3p", "person")
        ("cv", "subordinate")
        ("d_ma", "subordinate")
        ("part_acak", "subordinate")
        ("part_dik", "subordinate")
        ("part_yan", "subordinate")
        ("t_aor", "tense")
        ("t_cond", "tense")
        ("t_cont", "tense")
        ("t_fut", "tense")
        ("t_imp", "tense")
        ("t_makta", "tense")
        ("t_narr", "tense")
        ("t_obl", "tense")
        ("t_opt", "tense")
        ("t_past", "tense")
        ("caus", "voice")
        ("pass", "voice")
        ("rec", "voice")
        ("ref", "voice")
        ;

std::vector<std::string> HfstTurkishTagsParser::TAGS_ALLOWED_AS_POS =
    boost::assign::list_of
        ("pos");

HfstTurkishTagsParser::HfstTurkishTagsParser(const boost::program_options::variables_map&) {
}

HfstTurkishTagsParser::~HfstTurkishTagsParser() {
}

HfstTags HfstTurkishTagsParser::parse(HfstRawTags tags) {
    HfstTags attributes;
    RegExp regAttrib("<([^<>]*)>");
    std::map<std::string, std::string> attribs;
    for (HfstRawTags::iterator tagIt = tags.begin(); 
            tagIt != tags.end(); ++ tagIt) {
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

std::string HfstTurkishTagsParser::getAttributeName_(const std::string &value) {
    std::map<std::string, std::string>::iterator foundAttribute =
        PREDEFINED_TAGS.find(value);
    if (foundAttribute != PREDEFINED_TAGS.end())
        return foundAttribute->second;
    return "";
}

HfstTags HfstTurkishTagsParser::getLexemeAttributes(HfstRawTags tags) {
    HfstTags lexemeAttributes;
    HfstTags allAttributes = parse(tags);
    for (HfstTags::iterator attribIt = 
            allAttributes.begin(); attribIt != allAttributes.end(); ++ attribIt) {
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

HfstTags HfstTurkishTagsParser::getFormAttributes(HfstRawTags tags) {
    HfstTags formAttributes;
    HfstTags allAttributes = parse(tags);
    for (HfstTags::iterator attribsIt = 
            allAttributes.begin(); attribsIt != allAttributes.end(); ++ attribsIt) {
        std::map<std::string, std::string> attributes = *attribsIt;
        std::string pos = getPartOfSpeechTag_(attributes);
        attributes["pos"] = pos;
        formAttributes.push_back(attributes);
    }
    return formAttributes;
}

std::string HfstTurkishTagsParser::getPartOfSpeechTag_(
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
