#ifndef JSON_SITE_HDR
#define JSON_SITE_HDR

#include <string>

#include "template_site.hpp"

class JsonSite : public TemplateSite
{

public:

    JsonSite(PsiServer& server);

    char * jsonOutput();
    char * actionJson();
    char * actionJsonStupid();

private:

    std::ostringstream output_;

    void addKeyValuePairToOutput_(std::string, std::string, bool isLast=false);
    void addKeyValuePairToOutputAsString_(std::string, std::string, bool isLast=false);

    static const std::vector<std::string> JSON_WRITERS;
    bool isJavascriptOutput_(std::string pipe);

    static const std::string PIPE_KEY_NAME;
    static const std::string INPUT_KEY_NAME;
    static const std::string OUTPUT_KEY_NAME;
    static const std::string ERROR_KEY_NAME;

};

#endif
