#include "html_help_formatter.hpp"

#include "sundown/cpp/stringwrapper.hpp"

void HtmlHelpFormatter::doFormatOneProcessorHelp(
    std::string processorName,
    std::string description,
    boost::program_options::options_description options,
    std::ostream& output)
{
    output << "<h3>" << processorName << "</h3>" << std::endl;

    if (description.size() != 0) {
        output << "<p class=\"proc-desc\">" << markdownString2String(description) << "</p>" << std::endl;
    }

    output << "<pre class=\"proc-option\">" << options << "</pre>" << std::endl;
}

HtmlHelpFormatter::~HtmlHelpFormatter() { }
