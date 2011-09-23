#include <string>

#include "template_site.hpp"

class PipeSite : public TemplateSite
{

public:

    std::string pipe;
    std::string input;
    std::string output;
    std::string fileName;

    PipeSite(PsiServer& server, std::string initialPipe);

    char * inputText();
    char * pipeText();
    char * actionPipe();

    char * outputText();

private:

    static const std::string INITIAL_TEXT;

    std::string runPipe(std::string input);

};

