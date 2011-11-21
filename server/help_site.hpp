#include "template_site.hpp"

#include <boost/program_options/options_description.hpp>

class HelpSite : public TemplateSite
{

public:

    HelpSite(PsiServer& server, std::map<std::string,
        boost::program_options::options_description> descriptions);

    char * processorHelps();

private:

    std::map<std::string, boost::program_options::options_description> optionsDescriptions_;
    void registerIncludesAndActions();

};

