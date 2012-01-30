#include "cutter_annotator_options.hpp"

std::string getHardLimitDescription_(const std::string& edgeName);
std::string getSoftLimitDescription_(const std::string& edgeName);

void addCutterAnnotatorOptions(
    boost::program_options::options_description& optionsDescription,
    size_t hardLimit,
    size_t softLimit,
    const std::string& edgeName) {

    optionsDescription.add_options()
        ((edgeName + "-length-hard-limit").c_str(),
         boost::program_options::value<size_t>()->default_value(hardLimit),
         (getHardLimitDescription_(edgeName)).c_str())
        ((edgeName + "-length-soft-limit").c_str(),
         boost::program_options::value<size_t>()->default_value(softLimit),
         (getSoftLimitDescription_(edgeName)).c_str())
        ;
}

std::string getHardLimitDescription_(const std::string& edgeName) {
    return
        "maximum length (in bytes, not in characters) of a " + edgeName
        + " (if, according to rules, a " + edgeName +
        + " of a greater length would be generated, a " + edgeName
        + " break is forced), zero turns the limit off";
}

std::string getSoftLimitDescription_(const std::string& edgeName) {
    return
        "soft limit on the length (in bytes) of a " + edgeName
        + " (" + edgeName
        + " break is forced only on spaces), zero turns the limit off";
}
