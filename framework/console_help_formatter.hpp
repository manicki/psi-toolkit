#ifndef CONSOLE_HELP_FORMATTER_HDR
#define CONSOLE_HELP_FORMATTER_HDR

#include "help_formatter.hpp"

class ConsoleHelpFormatter : HelpFormatter {

private:

    void formatOneProcessorHelp(
        std::string processorName,
        boost::program_options::options_description options,
        std::ostream output);

};

#endif

