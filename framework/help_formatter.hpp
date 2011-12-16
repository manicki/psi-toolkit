#ifndef HELP_FORMATTER_HDR
#define HELP_FORMATTER_HDR

#include <iostream>
#include <boost/program_options/options_description.hpp>

#include "main_factories_keeper.hpp"

class HelpFormatter {

public:

    void formatOneProcessorHelp(std::string processorName, std::ostream& output);
    void formatHelps(std::ostream& output);

    virtual ~HelpFormatter();

protected:

    virtual void doFormatOneProcessorHelp(
        std::string processorName,
        std::string description,
        boost::program_options::options_description options,
        std::ostream& output) =0;

    std::string getProcessorDescription(
        std::string processorName);

    boost::program_options::options_description getProcessorOptions(
        std::string processorName);
};

#endif