#include "html_help_formatter.hpp"

#include "sundown/cpp/stringwrapper.hpp"

void HtmlHelpFormatter::doFormatOneProcessorHelp(
    std::string processorName,
    std::string description,
    boost::program_options::options_description options,
    std::vector<TestBatch> usingExamples,
    std::ostream& output)
{
    output << "<div class=\"help-item\">"
        << "<h2>" << processorName << "</h2>" << std::endl;

    if (description.size() != 0) {
        output << "<div class=\"help-desc\">" << markdownString2String(description)
            << "</div>" << std::endl;
    }

    if (!usingExamples.empty()) {
        formatUsingExamples(usingExamples, output);
    }

    formatAllowedOptions(options, output);
    output << "</div>" << std::endl;
}

void HtmlHelpFormatter::formatUsingExamples(std::vector<TestBatch> batches,
    std::ostream& output)
{
    output << "<div class=\"help-example\">"
        << "<h3>" << EXAMPLES_HEADER << "</h3>" << std::endl;

    for (unsigned int i = 0; i < batches.size(); i++) {
       output << "<pre class=\"example-pipe\">"
            << batches[i].getPipeline() << "</pre>" << std::endl;

       output << "<div class=\"example-desc\">"
            << markdownString2String(batches[i].getDescription()) << "</div>" << std::endl;

        std::vector<TestRun> inOuts = batches[i].getTestRuns();
        for (unsigned int j = 0; j < inOuts.size(); j++) {
            output << "<div class=\"in-out\">" << std::endl;

            std::string fileContent = getFileContent(inOuts[j].getInputFilePath());
            output << "<div class=\"in\">in:</div>"
                << "<pre>" << fileContent << "</pre>" << std::endl;

            fileContent = getFileContent(inOuts[j].getExpectedOutputFilePath());
            output << "<div class=\"out\">out:</div>"
                << "<pre>" << fileContent << "</pre>" << std::endl;
            output << "</div>" << std::endl;
        }
    }

    output << "</div>" << std::endl;
}

void HtmlHelpFormatter::formatAllowedOptions(boost::program_options::options_description options,
    std::ostream& output) {

    output << "<div class=\"help-opts\">"
        << "<h3>" << OPTIONS_HEADER << "</h3>" << std::endl
        << "<pre>" << options << "</pre>" << std::endl
        << "</div>" << std::endl;
}

HtmlHelpFormatter::~HtmlHelpFormatter() { }
