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

    std::string recognizeType(const std::string &);
    std::string recognizeExtension(const std::string &);

    static const std::string UNKNOWN_TYPE;
private:

#if HAVE_LIBMAGIC
    magic_t magicCookie_;
#endif

    std::string getFileType_(const std::string& magicFileInfo);
    std::string getFileExtension_(const std::string& magicFileInfo);

    static std::map<std::string, std::string> magicFileTypeToFileExtension_;
};

#endif
