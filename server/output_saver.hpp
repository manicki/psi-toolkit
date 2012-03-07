#include <string>

#include "guid_generator.hpp"

class OutputSaver
{

public:

    OutputSaver(std::string rootPath);
    std::string storeOutput(std::string content);

private:

    GUIDGenerator guidGenerator_;
    std::string websiteRoot_;

    static std::string FILE_EXTENSION;
    static std::string STORAGE_DIR;

};
