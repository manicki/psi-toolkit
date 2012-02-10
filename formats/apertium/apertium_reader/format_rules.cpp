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

    if (inputEncoding != "UTF-8") {
        WARN("input encoding specified in format options is " << inputEncoding);
    }
    if (outputEncoding != "UTF-8") {
        WARN("ouput encoding specified in format options is " << outputEncoding);
    }
}

bool FormatOptions::isCaseSensitive() {
    return caseSensitive_;
}

//FormatRule

FormatRule::FormatRule(const std::string& type, bool eos, int priority,
    const std::string& beginEnd)
        : type_(type), eos_(eos), priority_(priority), beginEnd_(beginEnd) {
}

FormatRule::FormatRule(const std::string& type, bool eos, int priority,
    const std::string& begin, const std::string& end)
        : type_(type), eos_(eos), priority_(priority), begin_(begin), end_(end) {
}

std::string FormatRule::getType() {
    return type_;
}

bool FormatRule::isEos() {
    return eos_;
}

int FormatRule::getPriority() {
    return priority_;
}

//ReplacementRule

ReplacementRule::ReplacementRule(const std::string& regexp)
    : regexp_(regexp) {
}

void ReplacementRule::addReplacement(std::string source, std::string target) {
    sourceToTargetMap_.insert(std::pair<std::string, std::string>(source, target));
}

std::string ReplacementRule::getRegexp() {
    return regexp_;
}
