#include <boost/foreach.hpp>

#include "format_specification.hpp"
#include "object_cache.hpp"
#include "regexp.hpp"
#include "logging.hpp"


//FormatSpecification

FormatSpecification::FormatSpecification(FormatOptions options,
        std::pair<std::vector<FormatRule>, std::vector<ReplacementRule> > rules)
            : formatOptions_(options),
            formatRules_(rules.first), replacementRules_(rules.second) { }

FormatOptions FormatSpecification::getOptions() {
    return formatOptions_;
}

std::vector<FormatRule> FormatSpecification::getFormatRules() {
    return formatRules_;
}

std::vector<ReplacementRule> FormatSpecification::getReplacementRules() {
    return replacementRules_;
}


//FormatSpecificationReader

FormatSpecificationReader::FormatSpecificationReader(const boost::filesystem::path& filePath)
    : xmlParsed_(ObjectCache::getInstance().getObject<XmlPropertyTree>(filePath)) {

    SET_LOGGING_LEVEL("DEBUG");
}

FormatSpecification FormatSpecificationReader::readFormatSpecification() {
    FormatSpecification formatSpecification(parseOptions_(), parseRules_());

    return formatSpecification;
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

    return std::pair<std::vector<FormatRule>, std::vector<ReplacementRule> >
        (formatRules, replacementRules);
}

FormatRule FormatSpecificationReader::parseFormatRule_(boost::property_tree::ptree& ruleNode) {
    std::string type = ruleNode.get<std::string>("<xmlattr>.type");
    std::string eos = ruleNode.get<std::string>("<xmlattr>.eos");
    int priority = ruleNode.get<int>("<xmlattr>.priority");

    DEBUG("found format rule: " << type << "; " << eos << "; " << priority);

    //FIXME: according to documentation, this node should be named 'begin-end' instead of 'tag'
    std::string regexpOne = ruleNode.get<std::string>("tag.<xmlattr>.regexp", "");

    if (!regexpOne.empty()) {
        DEBUG("  with regexp: " << regexpOne);
        return FormatRule(type, yesNoToBool_(eos), priority, regexpOne);
    }
    else {
        regexpOne = ruleNode.get<std::string>("begin.<xmlattr>.regexp");
        std::string regexpTwo = ruleNode.get<std::string>("end.<xmlattr>.regexp");

        DEBUG("  with regexp: " << regexpOne << "; " << regexpTwo);
        return FormatRule(type, yesNoToBool_(eos), priority, regexpOne, regexpTwo);
    }
}

ReplacementRule FormatSpecificationReader::parseReplacementRule_(
    boost::property_tree::ptree& ruleNode) {

    std::string regexp = ruleNode.get<std::string>("<xmlattr>.regexp");
    ReplacementRule rule(regexp);

    //FIXME: how to iterate by the subnode in given node?
    //in this solution only the first <replace> node is processed
    BOOST_FOREACH(boost::property_tree::ptree::value_type & v, ruleNode.get_child("replace")) {
        std::string source = v.second.get<std::string>("source");
        std::string target = v.second.get<std::string>("target");

        rule.addReplacement(source, target);
    }

    DEBUG("found replacement rule: " << regexp << " with " << rule.replacementsCount()
        << " replacements");

    return rule;
}

bool FormatSpecificationReader::yesNoToBool_(std::string& yesNo) {
    return (yesNo == "yes" ? true : false);
}
