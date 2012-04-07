#include <boost/algorithm/string/join.hpp>

#include "format_rules.hpp"
#include "logging.hpp"


//FormatOptions

FormatOptions::FormatOptions(int largeblocksSize,
    const std::string& inputEncoding, const std::string& outputEncoding,
    const std::string& escapeChars, const std::string& spaceChars,
    bool caseSensitive)
        : largeblocksSize_(largeblocksSize),
        escapeChars_(escapeChars), spaceChars_(spaceChars),
        caseSensitive_(caseSensitive) {

    if (inputEncoding != "UTF-8")
        WARN("input encoding specified in format options is " << inputEncoding);
    if (outputEncoding != "UTF-8")
        WARN("ouput encoding specified in format options is " << outputEncoding);
}

bool FormatOptions::isCaseSensitive() {
    return caseSensitive_;
}

//FormatRule

FormatRule::FormatRule(const std::string& type, bool eos, int priority)
        : type_(type), eos_(eos), priority_(priority) { }

FormatRule::FormatRule(const std::string& type, bool eos, int priority,
    const std::string& begin, const std::string& end)
        : type_(type), eos_(eos), priority_(priority), begin_(begin), end_(end) { }

void FormatRule::addRule(const std::string& ruleRegexp) {
    tags_.push_back(ruleRegexp);
}

std::string FormatRule::getType() {
    return type_;
}

bool FormatRule::getEos() {
    return eos_;
}

int FormatRule::getPriority() const {
    return priority_;
}

std::string FormatRule::getRegexp() {
    return std::string("(?:\\s*?)") + getRegexp_() + "(?:\t*)";
}

std::string FormatRule::getRegexp_() {
    if (!tags_.empty()) {
        return tagsToRegexpDisjunctions_();
    }
    return begin_ + "(?:(?:\\s|.)*?)" + end_;
}

std::string FormatRule::tagsToRegexpDisjunctions_() {
    if (tags_.size() == 1) {
        return tags_[0];
    }
    return std::string("(?:") + boost::algorithm::join(tags_, ")|(?:") + ")";
}

bool FormatRule::operator< (const FormatRule &other) const {
    return priority_ < other.getPriority();
}


//ReplacementRule

ReplacementRule::ReplacementRule(const std::string& regexp) : regexp_(regexp) { }

void ReplacementRule::addReplacement(std::string source, std::string target) {
    sourceToTargetMap_.insert(std::pair<std::string, std::string>(source, target));
}

void ReplacementRule::addPreferredReplacement(std::string target, std::string source) {
    preferredTargetToSourceMap_.insert(std::pair<std::string, std::string>(target, source));
}

std::string ReplacementRule::getRegexp() {
    return regexp_;
}

const std::map<std::string, std::string>* ReplacementRule::sourceToTargetMap() {
    return &sourceToTargetMap_;
}

