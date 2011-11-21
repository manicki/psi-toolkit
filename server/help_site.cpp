#include "help_site.hpp"
#include "logging.hpp"

HelpSite::HelpSite(PsiServer& server,
    std::map<std::string, boost::program_options::options_description> descriptions)
    : TemplateSite(server), optionsDescriptions_(descriptions)
{
    registerIncludesAndActions();
}

void HelpSite::registerIncludesAndActions() {
    psiServer_.registerIncludeCode(
        "help_site_processor_helps", boost::bind(&HelpSite::processorHelps, this));
}

char * HelpSite::processorHelps() {
    std::string help = "fake help";
    return stringToChar(help);
}


