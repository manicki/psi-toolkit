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
        : type_(type), eos_(eos), priority_(priority) {
}

void FormatRule::addRule(const std::string& ruleRegexp) {
    tags_.push_back(ruleRegexp);
}

FormatRule::FormatRule(const std::string& type, bool eos, int priority,
    const std::string& begin, const std::string& end)
        : type_(type), eos_(eos), priority_(priority), begin_(begin), end_(end) { }

std::string FormatRule::getType() {
    return type_;
}

bool FormatRule::isEos() {
    return eos_;
}

int FormatRule::getPriority() const {
    return priority_;
}

std::string FormatRule::getRegexp() {
    std::string coreRegexp;

    //TODO: refaktoryzacja
    if (!tags_.empty()) {
        coreRegexp = tagsToRegexAlternative_();
    }
    else {
        coreRegexp = begin_ + "(?:(?:\\s|.)*?)" + end_;
    }

    std::string fullRegexp = "(?:\t*?)"; //"(?:\\s*?)";
    fullRegexp += coreRegexp;
    fullRegexp += "(?:\t*)"; //"(?:\\s*)";

    return fullRegexp;
}

std::string FormatRule::tagsToRegexAlternative_() {
    if (tags_.size() == 1) {
        return tags_[0];
    }

    std::string regexAlternative = "(";
    for (unsigned int i = 0; i < tags_.size(); i++) {
        if (i != 0) regexAlternative += ")|(";
        regexAlternative += tags_[i];
    }
    regexAlternative += ")";

    return regexAlternative;
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

