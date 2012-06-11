#ifndef HTML_HELP_FORMATTER_HDR
#define HTML_HELP_FORMATTER_HDR

#include "help_formatter.hpp"

class HtmlHelpFormatter : public HelpFormatter {

public:
    ~HtmlHelpFormatter();

private:

    void doFormatOneProcessorHelp(
        std::string processorName,
        std::string description,
        boost::program_options::options_description options,
        std::vector<TestBatch> usingExamples,
        std::ostream& output);

    void formatUsingExamples_(std::vector<TestBatch> batches, std::ostream& output);

    void formatAllowedOptions_(boost::program_options::options_description options,
        std::ostream& output);
    void doFormatOneAlias(
        std::string aliasName,
        std::list<std::string> processorNames,
        std::ostream& output);
    void doFormatHelpIntroduction(std::string text, std::ostream& output);
    void doFormatTutorial(std::string text, std::ostream& output);
};

#endif
