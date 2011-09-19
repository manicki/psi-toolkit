#include <string>

#include "template_site.hpp"

class PipeSite : public TemplateSite
{

public:

    std::string pipe;
    std::string input;
    std::string output;

    PipeSite(PsiServer& server, std::string initialPipe);

    char * inputText();
    char * actionInputText();

    char * pipeText();
    char * actionPipeText();

    char * outputText();

private:

    static const std::string INITIAL_TEXT;

    std::string runPipe(std::string input);

};

