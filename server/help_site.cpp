#include "help_site.hpp"
#include "logging.hpp"

#include "html_help_formatter.hpp"

HelpSite::HelpSite(PsiServer& server)
    : TemplateSite(server)
{
    psiServer_.registerIncludeCode(
        "help_site_processor_helps", boost::bind(&HelpSite::processorHelps, this));
}

char * HelpSite::processorHelps() {

    std::ostringstream streamForHelps;

    HelpFormatter* htmlFormatter = new HtmlHelpFormatter;
    htmlFormatter->formatHelps(streamForHelps);
    delete htmlFormatter;

    return stringToChar(streamForHelps.str());
}


