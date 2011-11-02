#include <string>

class OutputSaver
{

public:

//    OutputSaver();
    OutputSaver(std::string rootPath);
    std::string storeOutput(std::string content);

private:

    int guid;
    std::string websiteRoot;
    static std::string fileExtension;
    static std::string storageDir;

    std::string getNewGuid();

};

