#ifndef APERTIUM_DEFORMATTER_HDR
#define APERTIUM_DEFORMATTER_HDR

#include <string>

#include <boost/filesystem.hpp>

#include "regexp.hpp"
#include "format_specification.hpp"

struct DeformatIndex {
    int begin;
    int end;
    std::string info;

    DeformatIndex(std::pair<int, int> indexes, const std::string& str)
    : begin(indexes.first), end(indexes.second), info(str) { }

    int length() {
        return end - begin;
    }
};

class ApertiumDeformatter {
public:

    ApertiumDeformatter(const boost::filesystem::path& specFilePath);

    std::string deformat(const std::string& input);

private:

    FormatSpecification formatSpecification_;
    FormatSpecification initializeFormatSpecification_(const boost::filesystem::path& path);

    std::vector<DeformatIndex> processFormatRules_(const std::string &);
    void processReplacementRules_(const std::string &);
    std::string clearFromDeformatData_(const std::string& input,
        std::vector<DeformatIndex>& indexes);

    //fixme: jak dodać RE_Options do regexp.hpp?
    pcrecpp::RE_Options perlRegexpOptions_;
    int initialInputSize_;

    std::pair<int, int> getMatchedStringIndexes(PerlStringPiece currentInput, std::string);

    const static std::string DELIMITER_BEGIN;
    const static std::string DELIMITER_END;
};

#endif
