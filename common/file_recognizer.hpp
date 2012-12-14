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

    std::string recognizeMimeType(const std::string &/*data*/);
    std::string recognizeFileExtension(const std::string &/*data*/);

    void recognizeMimeTypeAndFileExtension(
        const std::string &/*data*/,
        std::string &type,
        std::string &extension);

    std::string recognizeCompressedFileFormat(const std::string &/*data*/);

    static const std::string UNKNOWN_TYPE;
    static const std::string UNKNOWN_EXTENSION;
private:

    void initializeLibMagic();
    void closeLibMagic();

#if HAVE_LIBMAGIC
    magic_t magicCookie_;
#endif

    std::string getMimeType_(const std::string& magicFileInfo);
    std::string getFileExtension_(const std::string& magicFileInfo);
    std::string recognizeFileFormat_(const std::string &data, const std::string &filetype);

    static std::map<std::string, std::string> mimeTypeToFileExtension_;

    bool hasDjvuFormatBeginning_(const std::string& text);
    bool looksLikePsiFormat_(const std::string& text);
    bool looksLikeUTTFormat_(const std::string& text);
};

#endif
