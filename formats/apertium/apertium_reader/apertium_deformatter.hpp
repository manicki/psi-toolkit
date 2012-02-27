#ifndef APERTIUM_DEFORMATTER_HDR
#define APERTIUM_DEFORMATTER_HDR

#include <string>

#include <boost/filesystem.hpp>

#include "regexp.hpp"
#include "format_specification.hpp"

class ApertiumDeformatter {
public:

    ApertiumDeformatter(const boost::filesystem::path& specFilePath);

    struct DeformatData {
        int begin;
        int end;
        std::string info;

        DeformatData(std::pair<int, int> indexes, const std::string& str)
        : begin(indexes.first), end(indexes.second), info(str) { }
    };

    std::string deformat(const std::string& input);

private:

    std::vector<DeformatData> deformatData_;
    std::string clearFormatData_(const std::string &);

    void processFormatRules_(const std::string &);
    void processReplacementRules_(const std::string &);

    FormatSpecification formatSpecification_;
    FormatSpecification initializeFormatSpecification_(const boost::filesystem::path& path);

    //fixme: jak dodać RE_Options do regexp.hpp?
    pcrecpp::RE_Options perlRegexpOptions_;
    int initialInputSize_;

    void storeDeformatData_(std::pair<int, int> indexes, const std::string& info);

    std::pair<int, int> getMatchedStringIndexes(PerlStringPiece currentInput, std::string);

    const static std::string DELIMITER_BEGIN;
    const static std::string DELIMITER_END;
};

#endif
