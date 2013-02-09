#include "hfst_finnish_tags_parser.hpp"
#include "regexp.hpp"
#include "logging.hpp"

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

const std::string HfstFinnishTagsParser::INNER_SEPARATOR = ",";

std::multimap<std::string, std::string> HfstFinnishTagsParser::PREDEFINED_TAGS =
    boost::assign::map_list_of
        ("pos", "adjective")
        ("pos", "adposition")
        ("pos", "adverb")
        ("pos", "conjunction")
        ("pos", "interjection")
        ("pos", "noun")
        ("pos", "numeral")
        ("pos", "particle")
        ("pos", "prefix")
        ("pos", "pronoun")
        ("pos", "verb")
        ("subcat", "card")
        ("subcat", "ord")
        ("subcat", "prefix")
        ("subcat", "proper")
        ("case", "nom")
        ("case", "gen")
        ("case", "par")
        ("case", "acc")
        ("case", "ine")
        ("case", "ela")
        ("case", "ill")
        ("case", "ade")
        ("case", "abl")
        ("case", "all")
        ("case", "ess")
        ("case", "tra")
        ("case", "ins")
        ("case", "abe")
        ("case", "cmt")
        ("inf", "a")
        ("inf", "ma")
        ("inf", "maisilla")
        ("pcp", "ma")
        ("pcp", "va")
        ("pcp", "nut")
        ("pcp", "neg")
        ("num", "sg")
        ("num", "pl")
        ("prs", "sg1")
        ("prs", "sg2")
        ("prs", "sg3")
        ("prs", "pl1")
        ("prs", "pl2")
        ("prs", "pl3")
        ("prs", "pe4")
        ("neg", "con")
        ("mood", "cond")
        ("mood", "impv")
        ("mood", "indv")
        ("mood", "potn")
        ("tense", "pres")
        ("tense", "past")
        ("voice", "act")
        ("voice", "pss")
        ("poss", "sg1")
        ("poss", "sg2")
        ("poss", "sg3")
        ("poss", "pl1")
        ("poss", "pl2")
        ("poss", "pl3")
        ("cmp", "pos")
        ("cmp", "cmp")
        ("cmp", "sup")
        ("drv", "ja")
        ("drv", "minen")
        ("drv", "inen")
        ("drv", "sti")
        ("drv", "ttain")
        ("num_case", "sg_nom")
        ("num_case", "sg_gen")
        ("num_case", "sg_acc")
        ("num_case", "pl_gen")
        ("num_case", "pl_acc")
        ("guess", "compound")
        ("guess", "derive")
        ("person", "pri")
        ("person", "sec")
        ("person", "ter")
        ("person", "impersonal")
        ("person", "neg")
        ;

std::vector<std::string> HfstFinnishTagsParser::TAGS_ALLOWED_AS_POS =
    boost::assign::list_of
        ("pos");

//@todo: split-pos-tags temporarily disabled as did not work as intended
//std::map<std::string, std::map<std::string, std::string> >
//HfstFinnishTagsParser::COMPLEX_POS_SPLIT =
//    boost::assign::map_list_of<std::string, std::map<std::string, std::string> >
//        ("verb", boost::assign::map_list_of
//            ("pcp", "verbinf")
//            ("inf", "verbinf")
//            ("drv", "verbinf")
//            ("prs", "verbfin")
//            ("person", "verbfin"));

std::vector<std::string> HfstFinnishTagsParser::INTERPRETATIONS_TO_SKIP_MARKS =
    boost::assign::list_of
        ("guess");

std::multimap<std::string, std::string> HfstFinnishTagsParser::ATTRIBUTE_VALUES_TO_SKIP =
    boost::assign::map_list_of
        ("case", "dis") //distributive case, e.g. aakkoset -> aakkosittain
        ("case", "lat")
        ("case", "sti");

std::vector<std::string> HfstFinnishTagsParser::ATTRIBUTES_TO_SKIP =
    boost::assign::list_of
        ("boundary");

const std::string HfstFinnishTagsParser::ENCLITIC_PARTICLE_TAG = "clit";

std::vector<std::string> HfstFinnishTagsParser::ENCLITIC_PARTICLE_VALUES =
    boost::assign::list_of
        ("han")
        ("kaan")
        ("kin")
        ("ko")
        ("pa")
        ("s");

std::vector<std::string> HfstFinnishTagsParser::ATTRIBUTES_INFLECTION_TYPES =
    boost::assign::list_of
        ("ktn") 
        ("kav");

std::vector<std::string> HfstFinnishTagsParser::MERGED_ATTRIBUTES =
    boost::assign::list_of
        ("prs")
        ("num_case");

std::map<std::string, std::map<std::string, std::string> >
HfstFinnishTagsParser::MERGED_ATTRIBUTE_VALUES =
    boost::assign::map_list_of<std::string, std::map<std::string, std::string> >
        ("sg1", boost::assign::map_list_of
            ("num", "sg")
            ("person", "pri"))
        ("sg2", boost::assign::map_list_of
            ("num", "sg")
            ("person", "sec"))
        ("sg3", boost::assign::map_list_of
            ("num", "sg")
            ("person", "ter"))
        ("pl1", boost::assign::map_list_of
            ("num", "pl")
            ("person", "pri"))
        ("pl2", boost::assign::map_list_of
            ("num", "pl")
            ("person", "sec"))
        ("neg", boost::assign::map_list_of
            ("person", "neg"))
        ("pe4", boost::assign::map_list_of
            ("person", "impersonal"))
        ("pl3", boost::assign::map_list_of
            ("num", "pl")
            ("person", "ter"))
        ("sg_nom", boost::assign::map_list_of
            ("num", "sg")
            ("case", "nom"))
        ("sg_acc", boost::assign::map_list_of
            ("num", "sg")
            ("case", "acc"))
        ("sg_gen", boost::assign::map_list_of
            ("num", "sg")
            ("case", "gen"))
        ("pl_acc", boost::assign::map_list_of
            ("num", "pl")
            ("case", "acc"))
        ("pl_gen", boost::assign::map_list_of
            ("num", "pl")
            ("case", "gen"))
        //temporary workaround
        ("sg_nom,sg_gen,sg_acc,pl_gen,pl_acc", boost::assign::map_list_of 
            ("num", "sg,pl")
            ("case", "nom,gen,acc"));

HfstFinnishTagsParser::HfstFinnishTagsParser(const boost::program_options::variables_map& options) {
    splitMergedAttributes_ = (options.count("keep-original-attributes") > 0) ? false : true;
    //@todo: split-pos-tags temporarily disabled as did not work as intended
    //splitPosTags_ = (options.count("do-not-split-pos-tags") > 0) ? false : true;
    includeEncliticParticles_ = (options.count("include-enclitic-particles") > 0) ? true : false;
    includeInflectionTypes_ = (options.count("include-inflection-types") > 0) ? true : false;
    //@todo: what about adding also --include-possessive-suffixes option ?
}

HfstFinnishTagsParser::~HfstFinnishTagsParser() {
}

HfstTags HfstFinnishTagsParser::parse(HfstRawTags tags) {
    HfstTags attributes;
    RegExp regFlag("@.+@");
    RegExp regSpec("\\[([^=]+)=([^=]+)\\]");
    for (HfstRawTags::iterator tagIt = tags.begin(); tagIt != tags.end(); 
            ++ tagIt) {
        std::string tag = *tagIt;
        if (RegExp::FullMatch(tag, regFlag))
            continue;
        std::string specKey = "";
        std::string specVal = "";
        if (RegExp::FullMatch(tag, regSpec, &specKey, &specVal)) {
            boost::algorithm::to_lower(specKey);
            boost::algorithm::to_lower(specVal);
            std::map<std::string, std::string> attribSpec = getAttributeSpecification_(specKey, specVal);
            for (std::map<std::string, std::string>::iterator specIt = attribSpec.begin();
                    specIt != attribSpec.end(); ++ specIt) {
                std::string attribName = specIt->first;
                std::string attribValue = specIt->second;
                if (skipAttribute_(attribName))
                    continue;
                if (isValidAttribute_(attribName)) {
                    if (skipAttributeValue_(attribName, attribValue))
                        continue;
                    HfstTags tempAttributes;
                    std::vector<std::string> singleValues;
                    boost::split(singleValues, attribValue, boost::is_any_of(INNER_SEPARATOR));
                    if (!attributes.empty()) {
                        for (HfstTags::iterator attribIt = attributes.begin(); 
                                attribIt != attributes.end(); ++attribIt) {
                                BOOST_FOREACH(std::string value, singleValues) {
                                    if (isValidAttributeValue_(attribName, value)) {
                                        std::map<std::string, std::string> attribs = *attribIt;
                                        addAttribute_(attribs, attribName, value);
                                        tempAttributes.push_back(attribs);
                                    } else
                                        DEBUG("Unknown value of attribute \"" << attribName << "\" [" << value << "]");
                                }
                            }
                    } else {
                        BOOST_FOREACH(std::string value, singleValues) {
                            if (isValidAttributeValue_(attribName, value)) {
                                std::map<std::string, std::string> attribs;
                                addAttribute_(attribs, attribName, value);
                                tempAttributes.push_back(attribs);
                            } else
                                DEBUG("Unknown value of attribute \"" << attribName << "\" [" << value << "]");
                        }
                    }
                    attributes = tempAttributes;
                } else
                    DEBUG("Unknown attribute [" << attribName << "]");
            }
        }
    }
    return attributes;
}

void HfstFinnishTagsParser::addAttribute_(std::map<std::string, std::string> &attribs,
        const std::string &attribute, const std::string &value) {
    if (includeEncliticParticles_ && ENCLITIC_PARTICLE_TAG == attribute) {
        std::map<std::string, std::string>::iterator foundEnclitic = 
            attribs.find(attribute);
        if (foundEnclitic == attribs.end())
            attribs.insert(std::pair<std::string, std::string>(attribute, value));
        else {
            std::string encliticsString = foundEnclitic->second;
            encliticsString += "+" + value;
            attribs[attribute] = encliticsString;
        }
    } else
        attribs.insert(std::pair<std::string, std::string>(attribute, value));
}

bool HfstFinnishTagsParser::skipAttribute_(const std::string &attribute) {
    BOOST_FOREACH(std::string attr, ATTRIBUTES_TO_SKIP) {
        if (attr == attribute)
            return true;
    }
    if (!includeEncliticParticles_ && ENCLITIC_PARTICLE_TAG == attribute)
        return true;
    if (!includeInflectionTypes_) {
        BOOST_FOREACH(std::string attrType, ATTRIBUTES_INFLECTION_TYPES) {
            if (attribute == attrType)
                return true;
        }
    }
    return false;
}

bool HfstFinnishTagsParser::skipAttributeValue_(const std::string &attribute,
        const std::string &value) {
    std::pair<std::multimap<std::string, std::string>::iterator,
        std::multimap<std::string, std::string>::iterator> it =
            ATTRIBUTE_VALUES_TO_SKIP.equal_range(attribute);
    for (std::multimap<std::string, std::string>::iterator valIt =
            it.first; valIt != it.second; ++ valIt) {
        if (valIt->second == value)
            return true;
    }
    return false;
}

bool HfstFinnishTagsParser::isValidAttribute_(const std::string &attribute) {
    if (PREDEFINED_TAGS.find(attribute) != PREDEFINED_TAGS.end())
        return true;
    else {
        if (includeEncliticParticles_ && ENCLITIC_PARTICLE_TAG == attribute)
            return true;
        if (includeInflectionTypes_) {
            BOOST_FOREACH(std::string attrType, ATTRIBUTES_INFLECTION_TYPES) {
                if (attribute == attrType)
                    return true;
            }
        }
        return false;
    }
}

bool HfstFinnishTagsParser::isValidAttributeValue_(const std::string &attribute,
        const std::string &value) {
    std::pair<std::multimap<std::string, std::string>::iterator,
        std::multimap<std::string, std::string>::iterator> it =
            PREDEFINED_TAGS.equal_range(attribute);
    for (std::multimap<std::string, std::string>::iterator valIt =
            it.first; valIt != it.second; ++ valIt) {
        if (valIt->second == value)
            return true;
    }
    if (it.first == it.second) { //no such attribute, whether it is not an 
                                 //enclitic particle or inflection type no
        if (attribute == ENCLITIC_PARTICLE_TAG) {
            BOOST_FOREACH(std::string encliticParticle, ENCLITIC_PARTICLE_VALUES) {
                if (encliticParticle == value)
                    return true;
            }
        } else {
            BOOST_FOREACH(std::string attr, ATTRIBUTES_INFLECTION_TYPES) {
                if (attr == attribute) {
                    RegExp regKtn("[1-7]?[0-9]");
                    RegExp regKav("[a-m]");
                    if (RegExp::FullMatch(value, regKtn) ||
                            RegExp::FullMatch(value, regKav))
                        return true;
                }
            }
        }
    }
    return false;
}

std::map<std::string, std::string> HfstFinnishTagsParser::getAttributeSpecification_(
        const std::string &key, const std::string &value) {
    if (splitMergedAttributes_) {
        BOOST_FOREACH(std::string attribute, MERGED_ATTRIBUTES) {
            if (attribute == key) {
                std::map<std::string, std::map<std::string, std::string> >::iterator valIt = 
                    MERGED_ATTRIBUTE_VALUES.find(value);
                if (valIt != MERGED_ATTRIBUTE_VALUES.end())
                    return valIt->second;
            } 
        }
    }
    std::map<std::string, std::string> result;
    result.insert(std::pair<std::string, std::string>(key, value));
    return result;
}

HfstTags HfstFinnishTagsParser::getLexemeAttributes(HfstRawTags tags) {
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

HfstTags HfstFinnishTagsParser::getFormAttributes(HfstRawTags tags) {
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

std::string HfstFinnishTagsParser::getPartOfSpeechTag_(
        std::map<std::string, std::string> attributes) {
    std::string posTag = "";
    BOOST_FOREACH(std::string pos, TAGS_ALLOWED_AS_POS) {
        if (!attributes[pos].empty()) {
            posTag = getActualPosTag_(attributes[pos], attributes);
            break;
        }
    }
    return posTag;
}

std::string HfstFinnishTagsParser::getActualPosTag_(
        const std::string &tag, std::map<std::string, std::string>) {
    std::string posTag = tag;
    //@todo: split-pos-tags temporarily disabled as did not work as intended
    //if (splitPosTags_) {
    //    std::map<std::string, std::map<std::string, std::string> >::iterator complexIt =
    //        COMPLEX_POS_SPLIT.find(tag);
    //    if (complexIt != COMPLEX_POS_SPLIT.end()) {
    //        std::map<std::string, std::string> splitPosList = complexIt->second;
    //        for (std::map<std::string, std::string>::iterator splitPosIt = 
    //                splitPosList.begin(); splitPosIt != splitPosList.end(); ++ splitPosIt) {
    //            if (!attributes[splitPosIt->first].empty()) {
    //                posTag = splitPosIt->second;
    //                break;
    //            }
    //        }
    //    }
    //}
    return posTag;
}
