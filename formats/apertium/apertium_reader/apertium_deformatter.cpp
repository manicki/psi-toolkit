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

    pcrecpp::RE re("(\\[.*?\\])");

    if (re.error().length() > 0) {
        ERROR("PCRE compilation in apertium-deformatter failed with error: " << re.error());
    }

    pcrecpp::StringPiece inputToSearch(input);
    std::string matched;
    while (re.FindAndConsume(&inputToSearch, &matched)) {
        DEBUG("matched: " << matched);
    }

    return output;
}

const std::string DELIMITER_BEGIN = "[";
const std::string DELIMITER_END = "]";
