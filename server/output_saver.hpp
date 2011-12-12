#include <string>

#include "guid_generator.hpp"

class OutputSaver
{

public:

    OutputSaver(std::string rootPath);
    std::string storeOutput(std::string content);

private:

    GUIDGenerator guidGenerator;

    std::string websiteRoot;
    static std::string fileExtension;
    static std::string storageDir;

};

