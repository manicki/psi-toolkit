#include "help_site.hpp"
#include "logging.hpp"

HelpSite::HelpSite(PsiServer& server) : TemplateSite(server)
{
    htmlHelpFormatter_ = HtmlHelpFormatter();

    psiServer_.registerIncludeCode(
        "help_site_description", boost::bind(&HelpSite::description, this));
    psiServer_.registerIncludeCode(
        "help_site_processor_helps", boost::bind(&HelpSite::processorHelps, this));
    psiServer_.registerIncludeCode(
        "help_site_aliases", boost::bind(&HelpSite::aliases, this));
    psiServer_.registerIncludeCode(
        "help_site_tutorial", boost::bind(&HelpSite::tutorial, this));
    psiServer_.registerIncludeCode(
        "help_site_licence", boost::bind(&HelpSite::licence, this));
    psiServer_.registerIncludeCode(
        "help_site_psi_format", boost::bind(&HelpSite::psiFormat, this));
    psiServer_.registerIncludeCode(
        "help_site_documentation_menu", boost::bind(&HelpSite::documentationMenu, this));

    psiServer_.registerIncludeCode(
        "help_site_pipeline_examples", boost::bind(&HelpSite::pipelineExamples, this));
}

char * HelpSite::description() {
    std::ostringstream streamForIntro;
    htmlHelpFormatter_.formatDescription(streamForIntro);

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

char * HelpSite::tutorial() {
    std::ostringstream streamForTutorial;
    htmlHelpFormatter_.formatTutorial(streamForTutorial);

    return stringToChar(streamForTutorial.str());
}

char * HelpSite::licence() {
    std::ostringstream streamForLicence;
    htmlHelpFormatter_.formatLicence(streamForLicence);

    return stringToChar(streamForLicence.str());
}

char * HelpSite::psiFormat() {
    std::ostringstream streamForPsi;
    htmlHelpFormatter_.formatAboutPsiFormat(streamForPsi);

    return stringToChar(streamForPsi.str());
}

char * HelpSite::documentationMenu() {
    std::ostringstream streamForMenu;
    htmlHelpFormatter_.formatDocumentationMenu(streamForMenu);

    return stringToChar(streamForMenu.str());
}

char * HelpSite::pipelineExamples() {
    std::ostringstream streamForExamples;
    htmlHelpFormatter_.formatPipelineExamplesInJSON(streamForExamples);

    return stringToChar(streamForExamples.str());
}
