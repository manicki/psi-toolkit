#include <boost/algorithm/string/join.hpp>
#include <boost/foreach.hpp>

#include "format_specification.hpp"
#include "object_cache.hpp"
#include "regexp.hpp"
#include "escaping.hpp"
#include "logging.hpp"


//FormatSpecification

FormatSpecification::FormatSpecification(std::string name, FormatOptions options,
        std::pair<std::vector<FormatRule>, std::vector<ReplacementRule> > rules)
            : name_(name), formatOptions_(options),
            formatRules_(rules.first), replacementRules_(rules.second) { }

std::string FormatSpecification::formatRulesRegexp() {
    std::string regexp = "";

    for (unsigned int i = 0; i < formatRules_.size(); ++i) {
        regexp += "(";
        regexp += formatRules_[i].getRegexp() + ")";

        if (i < (formatRules_.size() - 1)) {
            regexp += "|";
        }
    }

    return regexp;
}

int FormatSpecification::formatRuleSize() {
    return formatRules_.size();
}


FormatOptions FormatSpecification::getOptions() {
    return formatOptions_;
}

FormatRule FormatSpecification::getFormatRule(int i) {
    return formatRules_[i];
}

ReplacementRule FormatSpecification::getReplacementRule(int i) {
    return replacementRules_[i];
}


//FormatSpecificationReader

FormatSpecificationReader::FormatSpecificationReader(const boost::filesystem::path& filePath)
    : xmlParsed_(ObjectCache::getInstance().getObject<XmlPropertyTree>(filePath)) {

    SET_LOGGING_LEVEL("DEBUG");
}

FormatSpecification FormatSpecificationReader::readFormatSpecification() {
    FormatSpecification formatSpecification(parseName_(), parseOptions_(), parseRules_());

    return formatSpecification;
}

std::string FormatSpecificationReader::parseName_() {
    return xmlParsed_->get<std::string>("format.<xmlattr>.name");
}

FormatOptions FormatSpecificationReader::parseOptions_() {
    int largeblocksSize = xmlParsed_->get<int>
        ("format.options.largeblocks.<xmlattr>.size"); //FIXME: are these nodes optional or not?

    std::string inputEncoding = xmlParsed_->get<std::string>
        ("format.options.input.<xmlattr>.encoding");
    std::string outputEncoding = xmlParsed_->get<std::string>
        ("format.options.output.<xmlattr>.encoding");

    std::string escapeChars = xmlParsed_->get<std::string>
        ("format.options.escape-chars.<xmlattr>.regexp");
    std::string spaceChars = xmlParsed_->get<std::string>
        ("format.options.escape-chars.<xmlattr>.regexp");

    std::string caseSensitive = xmlParsed_->get<std::string>
        ("format.options.case-sensitive.<xmlattr>.value");

    DEBUG("found options: " << largeblocksSize << "; " << inputEncoding << "; " << outputEncoding
        << "; " << escapeChars << "; " << spaceChars << "; " << caseSensitive);

    return FormatOptions(largeblocksSize, inputEncoding, outputEncoding,
        escapeChars, spaceChars, yesNoToBool_(caseSensitive));
}

std::pair<std::vector<FormatRule>, std::vector<ReplacementRule> >
    FormatSpecificationReader::parseRules_() {

    std::vector<FormatRule> formatRules;
    std::vector<ReplacementRule> replacementRules;

    BOOST_FOREACH(boost::property_tree::ptree::value_type & v,
        xmlParsed_->get_child("format.rules")) {

        if (v.first == "format-rule") {
            formatRules.push_back(parseFormatRule_(v.second));
        }
        else {
            if (v.first == "replacement-rule") {
                replacementRules.push_back(parseReplacementRule_(v.second));
            }
            else {
                UnexpectedElementException(v.first);
            }
        }
    }

    std::sort(formatRules.begin(), formatRules.end());

    return std::pair<std::vector<FormatRule>, std::vector<ReplacementRule> >
        (formatRules, replacementRules);
}

FormatRule FormatSpecificationReader::parseFormatRule_(boost::property_tree::ptree& ruleNode) {
    std::string type = ruleNode.get<std::string>("<xmlattr>.type");
    std::string eos = ruleNode.get<std::string>("<xmlattr>.eos");
    int priority = ruleNode.get<int>("<xmlattr>.priority");

    DEBUG("found format rule: " << type << "; " << eos << "; " << priority);

    std::string regexpOne = ruleNode.get<std::string>("begin.<xmlattr>.regexp", "");

    if (regexpOne.empty()) {

        FormatRule rule(type, yesNoToBool_(eos), priority);
        boost::property_tree::ptree::const_iterator end = ruleNode.end();

        for (boost::property_tree::ptree::const_iterator it = ruleNode.begin(); it != end; ++it) {
            //FIXME: according to documentation, this node should be named 'begin-end'
            //instead of 'tag'
            if (it->first == "tag") {

                regexpOne = it->second.get<std::string>("<xmlattr>.regexp", "");
                removeQuotations_(regexpOne);
                removeBackreferences_(regexpOne);

                DEBUG("  with regexp: " << regexpOne);
                rule.addRule(regexpOne);
            }
        }

        return rule;
    }
    else {
        std::string regexpTwo = ruleNode.get<std::string>("end.<xmlattr>.regexp");
        removeQuotations_(regexpOne);
        removeBackreferences_(regexpOne);
        removeQuotations_(regexpTwo);
        removeBackreferences_(regexpTwo);

        DEBUG("  with regexp: [" << regexpOne << "]; " << regexpTwo);
        return FormatRule(type, yesNoToBool_(eos), priority, regexpOne, regexpTwo);
    }
}

ReplacementRule FormatSpecificationReader::parseReplacementRule_(
    boost::property_tree::ptree& ruleNode) {

    std::string regex = ruleNode.get<std::string>("<xmlattr>.regexp");
    ReplacementRule rule(regex);

    boost::property_tree::ptree::const_iterator end = ruleNode.end();

    for (boost::property_tree::ptree::const_iterator it = ruleNode.begin(); it != end; ++it) {
        if (it->first == "replace") {
            std::string source = it->second.get<std::string>("<xmlattr>.source");
            std::string target = it->second.get<std::string>("<xmlattr>.target");

            rule.addReplacement(source, target);

            std::string prefer = it->second.get<std::string>("<xmlattr>.prefer", "");
            if (prefer == "yes") {
                rule.addPreferredReplacement(target, source);
            }
        }
    }

    DEBUG("found replacement rule: " << regex << " contains " << rule.replacementsCount()
        << " replacements");

    return rule;
}

bool FormatSpecificationReader::yesNoToBool_(std::string& yesNo) {
    return (yesNo == "yes" ? true : false);
}

void FormatSpecificationReader::removeQuotations_(std::string& str) {
    size_t pos = 0;

    while ((pos = str.find("\"", pos)) != std::string::npos) {
        if (!Escaping::isEscaped(str, pos)) {
            str.erase(pos, 1);
            pos += 1;
        }
    }
}

void FormatSpecificationReader::removeBackreferences_(std::string& str) {
    size_t pos = 0;

    while ((pos = str.find("(", pos)) != std::string::npos) {
        if (!Escaping::isEscaped(str, pos) && !(pos+1 < str.length() && str[pos+1] == '?')) {
            str.replace(pos+1, 0, "?:");
            pos += 2;
        }
    }
}
