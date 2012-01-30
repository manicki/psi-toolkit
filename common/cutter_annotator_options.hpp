#ifndef CUTTER_ANNOTATOR_OPTIONS
#define CUTTER_ANNOTATOR_OPTIONS

#include <string>

#include <boost/program_options/options_description.hpp>

void addCutterAnnotatorOptions(
    boost::program_options::options_description& optionsDescription,
    size_t hard_limit,
    size_t soft_limit,
    const std::string& edgeName);

#endif
