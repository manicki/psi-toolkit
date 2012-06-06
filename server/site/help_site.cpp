#include "help_site.hpp"
#include "logging.hpp"

HelpSite::HelpSite(PsiServer& server) : TemplateSite(server)
{
    htmlHelpFormatter_ = HtmlHelpFormatter();

    psiServer_.registerIncludeCode(
        "help_site_introduction", boost::bind(&HelpSite::introduction, this));
    psiServer_.registerIncludeCode(
        "help_site_processor_helps", boost::bind(&HelpSite::processorHelps, this));
    psiServer_.registerIncludeCode(
        "help_site_aliases", boost::bind(&HelpSite::aliases, this));
}

char * HelpSite::introduction() {
    std::ostringstream streamForIntro;
    htmlHelpFormatter_.formatHelpIntroduction(streamForIntro);

    return stringToChar(streamForIntro.str());
}

char * HelpSite::processorHelps() {
    std::ostringstream streamForHelps;
    htmlHelpFormatter_.formatHelps(streamForHelps);

    return stringToChar(streamForHelps.str());
}

char * HelpSite::aliases() {
    std::ostringstream streamForAliases;
    htmlHelpFormatter_.formatAliases(streamForAliases);

    return stringToChar(streamForAliases.str());
}
