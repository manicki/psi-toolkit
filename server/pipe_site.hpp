#include <string>

#include "template_site.hpp"
#include "output_saver.hpp"

class PipeSite : public TemplateSite
{

public:

    //PipeSite(PsiServer& server);
    PipeSite(PsiServer& server, const std::string & pipe);
    //PipeSite(PsiServer& server, std::string input, std::string pipe);

    char * inputText();
    char * pipeText();
    char * actionPipe();
    char * outputText();

    char * hiddenOptions();
private:

    void registerIncludesAndActions();

    const std::string initialText;
    const std::string initialPipe;
    std::string initialOutput;

    std::string getOrSetDefaultData(const char* name, std::string initialValue);

    std::string getInput();
    std::string runPipe(std::string);

    OutputSaver outputSaver;
    void createFileFromOutput(std::string output);
};

