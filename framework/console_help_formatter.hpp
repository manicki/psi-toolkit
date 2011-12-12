#ifndef CONSOLE_HELP_FORMATTER_HDR
#define CONSOLE_HELP_FORMATTER_HDR

#include "help_formatter.hpp"

class ConsoleHelpFormatter : public HelpFormatter {

public:
    ~ConsoleHelpFormatter();

protected:
    void doFormatOneProcessorHelp(
        std::string processorName,
        std::string description,
        boost::program_options::options_description options,
        std::ostream& output);

};

#endif

