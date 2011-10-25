
#include "tagset.hpp"

#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>

#include <boost/lexical_cast.hpp>
#include <vector>
#include <map>

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

Tagset::Tagset() {
    initProperties();
}

Tagset::Tagset(std::string &filename) {
    initProperties();
    readFromFile(filename);
}

void Tagset::readFromFile (std::string &filename) {
    std::ifstream tagsetFile(filename.c_str());
    if (!tagsetFile) {
        std::cerr << "Could not open tagset file: " << filename << "." << std::endl;
        return;
    }

    bool attr = false;
    bool pos = false;
    bool open = false;

    Pattern regKeyVal( "\\s*([^\\s]+)\\s*=\\s*([^\\s].*[^\\s#])?\\s*(#.*)?" );
    Pattern regComment( "#.*" );
    Pattern regWhite( "\\s+" );
    Pattern regOpenClasses( "\\s*[^\\s]+(\\s\\s*[^\\s]+\\s*)*\\s*" );

    while (!tagsetFile.eof())
    {
        std::string line;
        getline(tagsetFile, line);

        if (line == "")
            continue;
        if (RegExp::FullMatch(line, regWhite))
            continue;


        if (line.find("[ATTR]") != std::string::npos) {
            attr = true;
            pos = false;
            open = false;
            continue;
        }
        if (line.find("[POS]") != std::string::npos) {
            attr = false;
            pos = true;
            open = false;
            continue;
        }
        if (line.find("[OPEN]") != std::string::npos) {
            attr = false;
            pos = false;
            open = true;
            continue;
        }

        if (RegExp::FullMatch(line, regComment))
            continue;

        RegExp::Replace(&line, regComment, "");
        std::string key_;
        std::string value_;
        if (RegExp::FullMatch(line, regKeyVal, &key_, &value_)) {
            if (attr) {
                std::string attrName = key_;
                std::string values = value_;
                parseAttribute(attrName, values);
            } else if (pos) {
                std::string posName = key_;
                std::string attrs = value_;
                parsePartOfSpeech(posName, attrs);
            }
        } else if (RegExp::FullMatch(line, regOpenClasses)) {
            if (open) {
                parseOpenClasses(line);
            } else {
                std::cerr << "Wrong line: " << line << std::endl;
            }
        } else {
            std::cerr << "Wrong line: " << line << std::endl;
        }
    }

    tagsetFile.close();

    generateProperMorphologyLabels();
}

bool Tagset::checkAttribute(std::string &attribute) {
    for (std::vector<std::string>::iterator i = attrList.begin();
        i != attrList.end(); ++ i) {
        if (*i == attribute) {
            return true;
        }
    }
    return false;
}

bool Tagset::checkAttribute(std::string &pos, std::string &attribute) {
    std::map<std::string, std::vector<std::string> >::iterator p =
        partsOfSpeech.find(pos);
    if (p != partsOfSpeech.end()) {
        std::vector<std::string>::iterator i = p->second.begin();
        while (i != p->second.end()) {
            if (*i == attribute) {
                return true;
            } else if (*i == ("[" + attribute + "]")) {
                return true;
            }
            ++ i;
        }
    }
    return false;
}

bool Tagset::checkPos(std::string &pos) {
    if (partsOfSpeech.find(pos) != partsOfSpeech.end())
        return true;
    else
        return false;
}

bool Tagset::checkAttributeValue(std::string &attribute, std::string &value) {
    std::string attr_tmp = attribute;
    if (attr_tmp[0] == '[' && attr_tmp[attr_tmp.size() - 1] == ']')
        attr_tmp = attr_tmp.substr(1, attr_tmp.size() - 2);
    std::map<std::string, std::vector<std::string> >::iterator p =
        attributes.find(attr_tmp);
    if (p != attributes.end()) {
        std::vector<std::string>::iterator i = p->second.begin();
        while (i != p->second.end()) {
            if (*i == value) {
                return true;
                break;
            }
            ++ i;
        }
    }
    return false;
}

bool Tagset::checkMorphology(std::string &morphology) {
    if (properMorphologyLabels.find(morphology) !=
            properMorphologyLabels.end())
        return true;
    else
        return false;
}

int Tagset::getNumberOfAttributes() const {
    return attrList.size();
}

size_t Tagset::size() const {
    return partsOfSpeech.size();
}

int Tagset::getAttributeIndex(std::string &attribute) {
    int index = 0;
    for (std::vector<std::string>::iterator i = attrList.begin();
            i != attrList.end(); ++ i) {
        if (*i == attribute) {
            return index;
        }
        index ++;
    }
    return -1;
}

std::string Tagset::getAttributeAtIndex(std::string &pos, int index) {
    if (partsOfSpeech.find(pos) == partsOfSpeech.end()) {
        return "";
    }
    std::vector<std::string> attributes = partsOfSpeech[pos];
    if ( (size_t)index < attributes.size()) {
        std::string attr = attributes[index];
        if ((attr[0] == '[') && (attr[attr.length() - 1] == ']'))
            attr = attr.substr(1, attr.length() - 2);
        return attr;
    } else {
        return "";
    }

}

std::vector<std::string> Tagset::getAttributeValues(std::string &attribute) {
    if (checkAttribute(attribute)) {
        return attributes[attribute];
    } else {
        std::vector<std::string> empty;
        return empty;
    }
}

#if _WITH_BONSAI_PARSEGRAPH
void Tagset::readDescFromFile(std::string &filename) {
    std::ifstream tagsetFile(filename.c_str());
    if (!tagsetFile) {
        std::cerr << "Could not open description file: " << filename << "." << std::endl;
        return;
    }

    Pattern regKeyVal( "\\s*([^\\s]+)\\s*=\\s*([^\\s].*)?\\s*(#.*)?" );
    Pattern regComment( "#.*" );
    Pattern regWhite( "\\s+" );
    std::string whiteChars(" \t\n");

    bool general = false;
    bool posflag = false;
    bool groups = false;

    bool includepos = false;
    desc_separator = "#";

    while (!tagsetFile.eof()) {
        std::string line;
        getline(tagsetFile, line);

        if (line == "")
            continue;
        if (RegExp::FullMatch(line, regWhite))
            continue;

        if (RegExp::FullMatch(line, regComment))
            continue;

        RegExp::Replace(&line, regComment, "");

        if (line.find("[GENERAL]") != std::string::npos) {
            general = true;
            posflag = false;
            groups = false;
            continue;
        }
        if (line.find("[POS]") != std::string::npos) {
            general = false;
            posflag = true;
            groups = false;
            continue;
        }
        if (line.find("[GROUPS]") != std::string::npos) {
            general = false;
            posflag = false;
            groups = true;
            continue;
        }

        if (general) {
            std::string key;
            std::string value;
            if (RegExp::FullMatch(line, regKeyVal, &key, &value)) {
                if (key == "order") {
                    desc_order.clear();
                    std::string values = value;
                    std::vector<std::string> valuesVector;
                    boost::split(valuesVector, values, boost::is_any_of(whiteChars));
                    for (std::vector<std::string>::iterator val_it = valuesVector.begin();
                            val_it != valuesVector.end(); ++ val_it) {
                        std::string val = boost::algorithm::trim_copy(*val_it);
                        if (val == "orth")
                            desc_order.push_back(DESC_ORTH);
                        else if (val == "base")
                            desc_order.push_back(DESC_BASE);
                        else if (val == "morpho")
                            desc_order.push_back(DESC_MORPHO);
                        else {
                            std::cerr << "Unknown order value: " << val << std::endl;
                        }
                    }
                    continue;
                } else if (key == "include-pos") {
                    if (value == "1")
                        includepos = true;
                    else if (value == "0")
                        includepos = false;
                    else
                        std::cerr << "Unknown value: " << value << " of option: " << key << std::endl;
                    continue;
                } else if (key == "desc-terminals") {
                    if (value == "1")
                        desc_terminals = true;
                    else if (value == "0")
                        desc_terminals = false;
                    else
                        std::cerr << "Unknown value: " << value << " of option: " << key << std::endl;
                    continue;
                } else if (key == "desc-nonterminals") {
                    if (value == "1")
                        desc_nonterminals = true;
                    else if (value == "0")
                        desc_nonterminals = false;
                    else
                        std::cerr << "Unknown value: " << value << " of option: " << key << std::endl;
                    continue;
                } else if (key == "separator") {
                    desc_separator = value;
                } else {
                    std::cerr << "Unknown option: " << key << std::endl;
                    continue;
                }
            }
        }

        if (posflag) {
            std::string pos;
            std::string values;
            if (RegExp::FullMatch(line, regKeyVal, &pos, &values)) {
                std::map<std::string, std::vector<std::string> >::iterator p = partsOfSpeech.find(pos);
                if (p == partsOfSpeech.end()) {
                    std::cerr << "Unknown part of speech: " << pos << std::endl;
                    continue;
                }

                std::vector<std::string> patterns;
                std::vector<std::string> pos_vals = p->second;
                std::vector<std::string> valuesVector;
                boost::split(valuesVector, values, boost::is_any_of(whiteChars));

                std::vector<int> desc;
                if (includepos) {
                    std::string pattern = "^(" + pos + ").*";
                    patterns.push_back(pattern);
                }
                for (std::vector<std::string>::iterator val_it = valuesVector.begin();
                        val_it != valuesVector.end(); ++ val_it) {
                    std::string val = boost::algorithm::trim_copy(*val_it);
                    std::map<std::string, std::vector<std::string> >::iterator q = attributes.find(val);
                    if (q == attributes.end()) {
                        std::cerr << "Unknown attribute: " << val << std::endl;
                        continue;
                    }
                    std::string pattern = "";
                    for (std::vector<std::string>::iterator it = q->second.begin();
                            it != q->second.end(); ++ it) {
                        if (pattern != "")
                            pattern += "|";
                        pattern += *it;
                    }
                    pattern = ".*(" + pattern;
                    pattern += ").*";
                    patterns.push_back(pattern);
                }
                token_description.insert(std::pair<std::string, std::vector<std::string> >(pos, patterns));
            }
        }

        if (groups) {
            std::string group;
            std::string values;
            if (RegExp::FullMatch(line, regKeyVal, &group, &values)) {

                std::vector<std::string> patterns;
                std::vector<std::string> valuesVector;
                boost::split(valuesVector, values, boost::is_any_of(whiteChars));

                for (std::vector<std::string>::iterator val_it = valuesVector.begin();
                        val_it != valuesVector.end(); ++ val_it) {
                    std::string val = boost::algorithm::trim_copy(*val_it);
                    std::map<std::string, std::vector<std::string> >::iterator p =
                        attributes.find(val);
                    if (p == attributes.end()) {
                        std::cerr << "Unknown attribute: " << val << std::endl;
                        continue;
                    }

                    std::string pattern = "";
                    for (std::vector<std::string>::iterator it = p->second.begin();
                            it != p->second.end(); ++ it) {
                        if (pattern != "")
                            pattern += "|";
                        pattern += *it;
                    }
                    pattern = ".*(" + pattern;
                    pattern += ").*";
                    patterns.push_back(pattern);
                }
                group_description.insert(
                        std::pair<std::string, std::vector<std::string> >(
                            group, patterns)
                        );
            }
        }
    }
    tagsetFile.close();
}

bool Tagset::containsDesc() {
    return (! token_description.empty());
}

std::vector<std::string> Tagset::getGroupDesc(std::string group) {
    std::map<std::string, std::vector<std::string> >::iterator p =
        group_description.find(group);
    if (p != group_description.end())
        return p->second;
    else {
        std::vector<std::string> a;
        return a;
    }
}

std::vector<std::string> Tagset::getTokenDesc(std::string pos) {
    std::map<std::string, std::vector<std::string> >::iterator p =
        token_description.find(pos);
    if (p != token_description.end())
        return p->second;
    else {
        std::vector<std::string> a;
        return a;
    }
}
#endif

std::map<std::string, std::string> Tagset::getAttributes(
        std::string morphologyString, bool buffer) {
    std::map<std::string, std::string> results;
    if (this->size() == 0)
        return results;

    if (buffer) {
        std::map<std::string, std::map<std::string, std::string> >::iterator bufIt =
            attributes_buffer.find(morphologyString);
        if (bufIt != attributes_buffer.end()) {
            return bufIt->second;
        }
    }
    std::string attrSeparator = ":";
    std::vector<std::string> valuesVector;
    boost::split(valuesVector, morphologyString, boost::is_any_of(attrSeparator));

    std::string pos = "";
    int attribute_start = 0;
    std::vector<std::string> attributes;
    for (std::vector<std::string>::iterator valueIt = valuesVector.begin();
            valueIt != valuesVector.end(); ++ valueIt) {
        std::string val = boost::algorithm::trim_copy(*valueIt);
        if (pos == "") {
            pos = val;
            if (checkPos(pos)) {
                results.insert(std::pair<std::string, std::string>("pos", pos));
            }
        } else {
            if (attributes.empty()) {
                std::map<std::string, std::vector<std::string> >::iterator pi =
                    partsOfSpeech.find(pos);
                attributes = pi->second;
            }
            std::vector<std::string>::iterator ai = attributes.begin() +
                attribute_start;
            while (ai != attributes.end()) {
                std::string attr = *ai;
                if (attr[0] == '[' && attr[attr.size() - 1] == ']')
                    attr = attr.substr(1, attr.size() - 2);
                if (checkAttributeValue(attr, val)) {
                    results.insert(std::pair<std::string, std::string>(
                                attr, val));
                    attribute_start ++;
                    break;
                } else {
                    attribute_start ++;
                    ++ ai;
                }
            }
        }
    }
    if (buffer) {
        attributes_buffer.insert(
                std::pair<std::string, std::map<std::string, std::string> >(
                    morphologyString, results)
                );
    }
    return results;
}

std::vector<std::string> Tagset::getOpenClasses() const {
    return open_classes;
}

        std::vector<std::string> Tagset::getPosMatching(std::string regexp) {
            Pattern reg(regexp);
            std::vector<std::string> result;
            for (std::map<std::string,
                    std::vector<std::string> >::iterator pos_it =
                    partsOfSpeech.begin();
                    pos_it != partsOfSpeech.end(); ++ pos_it) {
                if (RegExp::FullMatch(pos_it->first, reg))
                    result.push_back(pos_it->first);
            }
            return result;
        }

        std::vector<std::string> Tagset::getPosNotMatching(std::string regexp) {
            Pattern reg(regexp);
            std::vector<std::string> result;
            for (std::map<std::string,
                    std::vector<std::string> >::iterator pos_it =
                    partsOfSpeech.begin();
                    pos_it != partsOfSpeech.end(); ++ pos_it) {
                if (!RegExp::FullMatch(pos_it->first, reg))
                    result.push_back(pos_it->first);
            }
            return result;
        }

        std::vector<std::string> Tagset::getPosExcept(std::string pos) {
            std::vector<std::string> result;
            for (std::map<std::string,
                    std::vector<std::string> >::iterator pos_it =
                    partsOfSpeech.begin();
                    pos_it != partsOfSpeech.end(); ++ pos_it) {
                if (pos_it->first != pos)
                    result.push_back(pos_it->first);
            }
            return result;
        }

        std::vector<std::string> Tagset::getAttributeValuesMatching(
                std::string attribute, std::string regexp) {
            Pattern reg(regexp);
            std::vector<std::string> result;
            std::map<std::string, std::vector<std::string> >::iterator attr_it =
                attributes.find(attribute);
            if (attr_it != attributes.end()) {
                std::vector<std::string> values = attr_it->second;
                for (std::vector<std::string>::iterator val_it = values.begin();
                        val_it != values.end(); ++ val_it) {
                    if (RegExp::FullMatch(*val_it, reg))
                        result.push_back(*val_it);
                }
            }
            return result;
        }

        std::vector<std::string> Tagset::getAttributeValuesNotMatching(
                std::string attribute, std::string regexp) {
            RegExp reg(regexp);
            std::vector<std::string> result;
            std::map<std::string, std::vector<std::string> >::iterator attr_it =
                attributes.find(attribute);
            if (attr_it != attributes.end()) {
                std::vector<std::string> values = attr_it->second;
                for (std::vector<std::string>::iterator val_it = values.begin();
                        val_it != values.end(); ++ val_it) {
                    if (!RegExp::FullMatch(*val_it, reg))
                        result.push_back(*val_it);
                }
            }
            return result;
        }

        std::vector<std::string> Tagset::getAttributeValuesExcept(
                std::string attribute, std::string value) {
            std::vector<std::string> result;
            std::map<std::string, std::vector<std::string> >::iterator attr_it =
                attributes.find(attribute);
            if (attr_it != attributes.end()) {
                std::vector<std::string> values = attr_it->second;
                for (std::vector<std::string>::iterator val_it = values.begin();
                        val_it != values.end(); ++ val_it) {
                    if (*val_it != value)
                        result.push_back(*val_it);
                }
            }
            return result;
        }

        std::vector<std::string> Tagset::getAttributes() {
            std::vector<std::string> result;
            for (std::map<std::string, std::vector<std::string> >::iterator it =
                    attributes.begin(); it != attributes.end(); ++ it)
                result.push_back(it->first);
            return result;
        }

        std::vector<std::string> Tagset::getPartsOfSpeech() {
            std::vector<std::string> result;
            for (std::map<std::string, std::vector<std::string> >::iterator it =
                    partsOfSpeech.begin(); it != partsOfSpeech.end(); ++ it)
                result.push_back(it->first);
            return result;
        }


        void Tagset::parseAttribute(std::string &attribute,
                std::string &valuesString) {
            if (attributes.find(attribute) != attributes.end()) {
                std::cerr << "Attribute: " << attribute <<
                    " already defined. Skipping." << std::endl;
                return;
            }
            std::vector<std::string> valuesVector;
            boost::split(valuesVector, valuesString, boost::is_any_of(" \t\n"));

            std::vector<std::string> attrValues;
            for (std::vector<std::string>::iterator valueIt =
                    valuesVector.begin();
                    valueIt != valuesVector.end(); ++ valueIt) {
                std::string value = boost::algorithm::trim_copy(*valueIt);
                attrValues.push_back(value);
            }
            attrList.push_back(attribute);
            attributes.insert(
                    std::pair<std::string, std::vector<std::string> >(
                        attribute, attrValues )
                    );
        }

        void Tagset::parsePartOfSpeech(std::string &partOfSpeech,
                std::string &attributesString) {
            if (partsOfSpeech.find(partOfSpeech) != partsOfSpeech.end()) {
                std::cerr << "Part of speech: " << partOfSpeech
                    << " already defined. Skipping." << std::endl;
                return;
            }

            std::vector<std::string> posAttrs;
            if (attributesString != "") {
                std::vector<std::string> valuesVector;
                boost::split(valuesVector, attributesString,
                        boost::is_any_of(" \t\n"));

                for (std::vector<std::string>::iterator valueIt = valuesVector.begin();
                        valueIt != valuesVector.end(); ++ valueIt) {
                    std::string attribute = boost::algorithm::trim_copy(*valueIt);
                    posAttrs.push_back(attribute);
                }
            }
            partsOfSpeech.insert(
                    std::pair<std::string, std::vector<std::string> >(
                        partOfSpeech, posAttrs)
                    );
        }

        void Tagset::parseOpenClasses(std::string &openClassesString) {
            std::vector<std::string> valuesVector;
            boost::split(valuesVector, openClassesString,
                    boost::is_any_of(" \t\n"));

            for (std::vector<std::string>::iterator valueIt = valuesVector.begin();
                    valueIt != valuesVector.end(); ++ valueIt) {
                std::string val = boost::algorithm::trim_copy(*valueIt);
                std::string class_name = val;
                std::map<std::string, std::vector<std::string> >::iterator it =
                    partsOfSpeech.find(class_name);
                if (it != partsOfSpeech.end()) {
                    open_classes.push_back(class_name);
                } else {
                    std::cerr << "Unknown class: " <<
                        class_name << " given as an open class name." << std::endl;
                }
            }
        }

void Tagset::generateProperMorphologyLabels() {
    Pattern regOptional( "\\[([^\\s\\[\\]]+)\\]" );

    for (std::map<std::string, std::vector<std::string> >::iterator
            posIt = partsOfSpeech.begin(); posIt != partsOfSpeech.end(); ++ posIt) {
        std::vector<Morphology> morphologies;
        Morphology posMorphology;
        posMorphology.insert(std::pair<std::string, std::string>(
                    "pos", posIt->first));
        morphologies.push_back(posMorphology);
        for (std::vector<std::string>::iterator attrIt = posIt->second.begin();
                attrIt != posIt->second.end(); ++ attrIt) {
            bool optional = false;
            std::string attribute = *attrIt;
            std::string attribute_;
            if (RegExp::FullMatch(attribute, regOptional, &attribute_)) {
                attribute = attribute_;
                optional = true;
            }
            std::vector<std::string> attrValues = attributes[attribute];
            std::vector<Morphology> morphologiesTmp;
            for (std::vector<Morphology>::iterator morphologyIt =
                morphologies.begin(); morphologyIt != morphologies.end();
                ++ morphologyIt ) {
                for (std::vector<std::string>::iterator valueIt = attrValues.begin();
                        valueIt != attrValues.end(); ++ valueIt) {
                    Morphology newMorphology = *morphologyIt;
                    newMorphology.insert(std::pair<std::string, std::string>(
                                attribute, *valueIt));
                    morphologiesTmp.push_back(newMorphology);
                }
            }
            if (!optional) {
                morphologies = morphologiesTmp;
            }
            else {
                morphologies.insert(morphologies.end(), morphologiesTmp.begin(),
                        morphologiesTmp.end());
            }
        }

        for (std::vector<Morphology>::iterator morphologyIt = morphologies.begin();
                morphologyIt != morphologies.end(); ++ morphologyIt) {
            std::string morphoString = util::getMorphologyString(*morphologyIt);
            properMorphologyLabels.insert(morphoString);
        }
    }
}

void Tagset::initProperties() {
#if _WITH_BONSAI_PARSEGRAPH
    desc_terminals = false;
    desc_nonterminals = false;
    desc_separator = "#";
#endif
}



}

}

}

