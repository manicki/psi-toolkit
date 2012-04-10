#include "help_site.hpp"
#include "logging.hpp"

#include "html_help_formatter.hpp"

HelpSite::HelpSite(PsiServer& server)
    : TemplateSite(server)
{
    psiServer_.registerIncludeCode(
        "help_site_processor_helps", boost::bind(&HelpSite::processorHelps, this));
    psiServer_.registerIncludeCode(
        "help_site_aliases", boost::bind(&HelpSite::aliases, this));
}

char * HelpSite::processorHelps() {
    std::ostringstream streamForHelps;
    HtmlHelpFormatter().formatHelps(streamForHelps);

    return stringToChar(streamForHelps.str());
}

char * HelpSite::aliases() {
    std::ostringstream streamForAliases;
    HtmlHelpFormatter().formatAliases(streamForAliases);

    return stringToChar(streamForAliases.str());
}
