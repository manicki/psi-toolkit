#include <string>

#include "template_site.hpp"
#include "output_saver.hpp"
#include "file_recognizer.hpp"

class PipeSite : public TemplateSite
{

public:

    PipeSite(PsiServer& server, const std::string & pipe, const std::string & text);

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

    OutputSaver outputSaver_;
    FileRecognizer fileRecognizer_;

    void clearPreviousFileFromOutput();
    void createFileFromOutput(const std::string& output);

    std::string generateOutput_(const std::string& rawOuput);
};
