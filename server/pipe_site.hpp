#include <string>

#include "template_site.hpp"

class PipeSite : public TemplateSite
{

public:

    PipeSite(PsiServer& server);
    //PipeSite(PsiServer& server, std::string input, std::string pipe);

    char * inputText();
    char * pipeText();
    char * actionPipe();
    char * outputText();

    char * hiddenOptions();
private:

    static const std::string initialText;
    static const std::string initialPipe;

    std::string getOrSetDefaultData(const char* name, std::string initialValue);

    std::string getInput();
    std::string runPipe();
};

