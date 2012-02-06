#include "html_help_formatter.hpp"

#include "sundown/cpp/stringwrapper.hpp"

void HtmlHelpFormatter::doFormatOneProcessorHelp(
    std::string processorName,
    std::string description,
    boost::program_options::options_description options,
    std::vector<TestBatch> usingExamples,
    std::ostream& output)
{
    output << "<div class=\"help-item\">" << std::endl;
    output << "<h2>" << processorName << "</h2>" << std::endl;

    if (description.size() != 0) {
        output << "<div class=\"help-desc\">" << markdownString2String(description)
            << "</div>" << std::endl;
    }

    output << "<div class=\"help-opts\"><pre>" << options << "</pre></div>" << std::endl;
    output << "</div>" << std::endl;
}

HtmlHelpFormatter::~HtmlHelpFormatter() { }
