#ifndef APERTIUM_DEFORMATTER_HDR
#define APERTIUM_DEFORMATTER_HDR

#include <string>

#include <boost/filesystem.hpp>

#include "regexp.hpp"
#include "format_specification.hpp"

struct DeformatIndex {
    int begin;
    int end;
    std::string type;
    bool eos;

    DeformatIndex(std::pair<int, int> indexes, const std::string& str, bool attr)
    : begin(indexes.first), end(indexes.second), type(str), eos(attr) { }

    int length() {
        return end - begin;
    }
};

class ApertiumDeformatter {
public:

    ApertiumDeformatter(const boost::filesystem::path& specFilePath);

    std::string processReplacementRules(const std::string& input);
    std::vector<DeformatIndex> processFormatRules(const std::string& input);
    std::string deformat(const std::string& input);

private:

    FormatSpecification formatSpecification_;
    FormatSpecification initializeFormatSpecification_(const boost::filesystem::path& path);

    //FIXME: should the RE_Options be added to the regexp wrapper?
    pcrecpp::RE_Options perlRegexpOptions_;
    int initialInputSize_;

    std::string clearFromDeformatData_(const std::string& input,
        std::vector<DeformatIndex>& indexes);

    std::pair<int, int> getMatchedStringIndexes_(PerlStringPiece currentInput, std::string);

    std::string decompressFiles_(const std::string &);
};

#endif
