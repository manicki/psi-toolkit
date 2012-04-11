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
        std::vector<TestBatch> usingExamples,
        std::ostream& output);

    void doFormatOneAlias(
        std::string aliasName,
        std::list<std::string> processorNames,
        std::ostream& output);

    void doFormatHelpIntroduction(std::string text, std::ostream& output);

};

#endif
