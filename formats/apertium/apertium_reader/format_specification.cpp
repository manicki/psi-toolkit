#include "format_specification.hpp"
#include "object_cache.hpp"
#include "regexp.hpp"
#include "logging.hpp"


//FormatSpecification

FormatSpecification::FormatSpecification(FormatOptions options,
        std::vector<FormatRule> formatRules, std::vector<ReplacementRule> replacementRules)
            : formatOptions_(options),
            formatRules_(formatRules), replacementRules_(replacementRules) { }

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
    FormatSpecification formatSpecification(
        parseOptions_(), parseFormatRules_(), parseReplacementRules_());

    return formatSpecification;
}

FormatOptions FormatSpecificationReader::parseOptions_() {
    DEBUG("parse format options from xml file...");

    return FormatOptions(0, "", "", "", "", false);
}

std::vector<FormatRule> FormatSpecificationReader::parseFormatRules_() {
    DEBUG("parse format rules from xml file...");

    std::vector<FormatRule> rules;
    FormatRule fakeRule("", false, 0, "");
    rules.push_back(fakeRule);

    return rules;
}

std::vector<ReplacementRule> FormatSpecificationReader::parseReplacementRules_() {
    DEBUG("parse replacement rules from xml file...");

    std::vector<ReplacementRule> rules;
    ReplacementRule fakeRule("");
    rules.push_back(fakeRule);

    return rules;
}
