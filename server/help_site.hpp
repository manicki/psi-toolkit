#include "template_site.hpp"

#include <boost/program_options/options_description.hpp>

class HelpSite : public TemplateSite
{

public:
    HelpSite(PsiServer& servers);

    char * processorHelps();

};
