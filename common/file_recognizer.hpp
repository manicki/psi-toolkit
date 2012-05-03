#ifndef FILE_RECOGNIZER_HDR
#define FILE_RECOGNIZER_HDR

#include "config.hpp"

#include <string>
#include <map>

#if HAVE_LIBMAGIC
#include <magic.h>
#endif

class FileRecognizer {

public:
    FileRecognizer();
    ~FileRecognizer();

    std::string recognizeMimeType(const std::string &);
    std::string recognizeFileExtension(const std::string &);

    static const std::string UNKNOWN_TYPE;
private:

#if HAVE_LIBMAGIC
    magic_t magicCookie_;
#endif

    std::string getMimeType_(const std::string& magicFileInfo);
    std::string getFileExtension_(const std::string& magicFileInfo);

    static std::map<std::string, std::string> mimeTypeToFileExtension_;
};

#endif
