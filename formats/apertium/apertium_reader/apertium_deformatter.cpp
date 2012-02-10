#include "apertium_deformatter.hpp"

ApertiumDeformatter::ApertiumDeformatter(const boost::filesystem::path& specificationFile)
    : formatSpecification_(specificationFile) {

}

std::string ApertiumDeformatter::deformat(const std::string& input) {
    std::string output = "fake apertium output";

    return output;
}
