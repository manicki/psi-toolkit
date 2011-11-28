#include "html_help_formatter.hpp"

void HtmlHelpFormatter::doFormatOneProcessorHelp(
    std::string processorName,
    boost::program_options::options_description options,
    std::ostream& output)
{
    output << "<h3>" << processorName << "</h3>" << std::endl
        << "<pre class=\"proc-option\">" << options << "</pre>" << std::endl;
}

HtmlHelpFormatter::~HtmlHelpFormatter() { }
