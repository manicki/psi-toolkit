#ifndef OUTPUT_SAVER_HDR
#define OUTPUT_SAVER_HDR

#include <string>

#include "guid_generator.hpp"
#include "file_recognizer.hpp"
#include "md5.hpp"

class OutputSaver
{

public:

    OutputSaver(const std::string& rootPath);

    std::string storeOutput(const std::string& content);
    std::string storeOutput(const std::string& content, const std::string& ext);

private:

    GUIDGenerator guidGenerator_;
    std::string websiteRoot_;
    FileRecognizer fileRecognizer_;

    std::string filePathToStore_(const std::string& guid, const std::string& ext);
    std::string filePathForHtml_(const std::string& guid, const std::string& ext);

    static const int GUID_LENGTH;
    static const std::string DEFAULT_FILE_EXTENSION;
    static const std::string STORAGE_DIR;
};

#endif
