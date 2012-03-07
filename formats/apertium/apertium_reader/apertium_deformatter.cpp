#include <boost/foreach.hpp>

#include "regexp.hpp"
#include "logging.hpp"

#include "apertium_deformatter.hpp"

ApertiumDeformatter::ApertiumDeformatter(const boost::filesystem::path& specFilePath)
    : formatSpecification_(initializeFormatSpecification_(specFilePath)) {

    SET_LOGGING_LEVEL("DEBUG");

    perlRegexpOptions_.set_utf8(true);
//    perlRegexpOptions_.set_multiline(true);
//    perlRegexpOptions_.set_dotall(true);
    perlRegexpOptions_.set_caseless(!formatSpecification_.getOptions().isCaseSensitive());
}

FormatSpecification ApertiumDeformatter::initializeFormatSpecification_(
    const boost::filesystem::path& path) {

    FormatSpecificationReader formatSpecRdr(path);
    return formatSpecRdr.readFormatSpecification();
}

std::string ApertiumDeformatter::deformat(const std::string& input) {
    initialInputSize_ = 0;

    std::vector<DeformatIndex> deformatIndexes = processFormatRules_(input);
    processReplacementRules_(input);

    std::string output = clearFromDeformatData_(input, deformatIndexes);

    return output;
}

std::vector<DeformatIndex> ApertiumDeformatter::processFormatRules_(const std::string& input) {
    PerlStringPiece currentInput(input);
    initialInputSize_ = currentInput.size();

    std::string regexp = formatSpecification_.formatRulesRegexp();
    PerlRegExp re(regexp, perlRegexpOptions_);
    DEBUG("looking for: " << regexp);

    const int rulesSize = formatSpecification_.formatRuleSize();
    if (rulesSize > PerlRegExp::MAX_MATCHES)
        ERROR("the number of rules is larger than " << PerlRegExp::MAX_MATCHES << "!");

    const PerlArg *args[rulesSize];
    std::string matches[rulesSize];

    for (int i = 0; i < rulesSize; i++) {
        args[i] = new PerlArg(&matches[i]);
    }

    std::vector<DeformatIndex> deformatIndexes;

    while (PerlRegExp::FindAndConsumeN(&currentInput, re, args, rulesSize)) {
        for (int i = 0; i < rulesSize; i++) {
            if (!matches[i].empty()) {
                std::pair<int, int> indexes = getMatchedStringIndexes(currentInput, matches[i]);
                std::string info = formatSpecification_.getFormatRule(i).getType();

                deformatIndexes.push_back(DeformatIndex(indexes, info));

                DEBUG(matches[i] << " (" << indexes.first << ", " << indexes.second << ")"
                    << " as " << info);
            }
        }
    }

    return deformatIndexes;
}

void ApertiumDeformatter::processReplacementRules_(const std::string& input) {
    PerlStringPiece currentInput(input);

    std::string inputFake = input;
}

std::string ApertiumDeformatter::clearFromDeformatData_(const std::string& input,
    std::vector<DeformatIndex>& indexes) {

    std::string clearInput = input;

    int length = 0;
    for (unsigned int i = 0; i < indexes.size(); i++) {
        clearInput.erase(indexes[i].begin - length, indexes[i].length());
        length += indexes[i].length();
    }

    return clearInput;
}

const std::string DELIMITER_BEGIN = "[";
const std::string DELIMITER_END = "]";

std::pair<int, int> ApertiumDeformatter::getMatchedStringIndexes(
     PerlStringPiece currentInput, std::string matchedString) {

    return std::pair<int, int>(initialInputSize_ - currentInput.size() - matchedString.size(),
        initialInputSize_ - currentInput.size());
}
