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

};

#endif
