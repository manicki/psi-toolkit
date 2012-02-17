#include <boost/foreach.hpp>

#include "regexp.hpp"
#include "logging.hpp"

#include "apertium_deformatter.hpp"

ApertiumDeformatter::ApertiumDeformatter(const boost::filesystem::path& specFilePath)
    : formatSpecification_(initializeFormatSpecification_(specFilePath)) {

    SET_LOGGING_LEVEL("DEBUG");
}

FormatSpecification ApertiumDeformatter::initializeFormatSpecification_(
    const boost::filesystem::path& path) {

    FormatSpecificationReader formatSpecRdr(path);
    return formatSpecRdr.readFormatSpecification();
}

std::string ApertiumDeformatter::deformat(const std::string& input) {
    std::string output = "apertium-reader got input: [" + input + "]";

    BOOST_FOREACH(FormatRule formatRule, formatSpecification_.getFormatRules()) {
        pcrecpp::StringPiece inputToSearch(input);
        std::string matched;
        int inputLength = inputToSearch.size();

        std::string regexp = "(";
        regexp += formatRule.getRegexp();
        regexp += ")";



        pcrecpp::RE re(regexp);
        DEBUG("looking for: " << regexp);

        if (re.error().length() > 0) {
            ERROR("PCRE compilation in apertium-deformatter failed with error: " << re.error());
        }

        // początek i koniec dopasowania
        while (re.FindAndConsume(&inputToSearch, &matched)) {
            DEBUG("  " << matched << " "
                << "("<< inputLength - inputToSearch.size() - matched.size()
                << ", " << inputLength - inputToSearch.size() << ")");
        }
    }

    return output;
}

const std::string DELIMITER_BEGIN = "[";
const std::string DELIMITER_END = "]";

/*
std::pair<int, int> ApertiumDeformatter::getMatchBeginEndIndex(
     pcrecpp::StringPiece currentInput, std::string matchedString) {

    return std::pair<int, int>(inputSize_ - currentInput.size() - matchedString.size(),
        inputSize_ - currentInput.size());
}
*/
