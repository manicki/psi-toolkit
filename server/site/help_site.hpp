#include "template_site.hpp"
#include "html_help_formatter.hpp"

#include <boost/program_options/options_description.hpp>

class HelpSite : public TemplateSite
{

public:
    HelpSite(PsiServer& servers);

    char * introduction();
    char * processorHelps();
    char * aliases();
    char * tutorial();

private:
    HtmlHelpFormatter htmlHelpFormatter_;

};
