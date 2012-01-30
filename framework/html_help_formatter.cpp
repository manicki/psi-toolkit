#include "html_help_formatter.hpp"

#include "sundown/cpp/stringwrapper.hpp"

void HtmlHelpFormatter::doFormatOneProcessorHelp(
    std::string processorName,
    std::string description,
    boost::program_options::options_description options,
    std::ostream& output)
{
    output << "<h2>" << processorName << "</h2>" << std::endl;

    if (description.size() != 0) {
        output << "<div class=\"proc-desc\">" << markdownString2String(description)
            << "</div>" << std::endl;
    }

    output << "<div class=\"proc-option\"><pre>" << options << "</pre></div>" << std::endl;
}

HtmlHelpFormatter::~HtmlHelpFormatter() { }
