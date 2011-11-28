#include "console_help_formatter.hpp"

void ConsoleHelpFormatter::doFormatOneProcessorHelp(
    std::string processorName,
    boost::program_options::options_description options,
    std::ostream& output)
{
    output << processorName << std::endl << options;
}

ConsoleHelpFormatter::~ConsoleHelpFormatter() { }
