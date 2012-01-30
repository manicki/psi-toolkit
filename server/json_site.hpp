#include <string>

#include "template_site.hpp"

class JsonSite : public TemplateSite
{

public:

    std::string jsonOutputAsString;

    JsonSite(PsiServer& server);

    char * jsonOutput();
    char * actionJson();


private:

    static std::string PIPE_PARAMETER;
    static std::string INPUT_PARAMETER;

};
