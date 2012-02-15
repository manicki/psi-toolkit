#ifndef APERTIUM_DEFORMATTER_HDR
#define APERTIUM_DEFORMATTER_HDR

#include <string>

#include <boost/filesystem.hpp>

#include "format_specification.hpp"

class ApertiumDeformatter {
public:

    ApertiumDeformatter(const boost::filesystem::path& specFilePath);

    std::string deformat(const std::string& input);

private:

    FormatSpecification formatSpecification_;
    FormatSpecification initializeFormatSpecification_(const boost::filesystem::path& path);

    const static std::string DELIMITER_BEGIN;
    const static std::string DELIMITER_END;
};

#endif
