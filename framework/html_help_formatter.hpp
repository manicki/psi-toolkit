#ifndef HTML_HELP_FORMATTER_HDR
#define HTML_HELP_FORMATTER_HDR

#include "help_formatter.hpp"

class HtmlHelpFormatter : public HelpFormatter {

public:
    ~HtmlHelpFormatter();

    void formatPipelineExamplesInJSON(std::ostream& output);

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
    void doFormatLicence(std::string text, std::ostream& output);

    void formatPipelineExampleInJSON_(TestBatch batch, std::ostream& output);

    std::string escapeHTML_(const std::string& text);
    std::string escapeJSON_(std::string& text);
};

#endif
