#include <boost/assign.hpp>

#include "file_recognizer.hpp"
#include "logging.hpp"

const std::string FileRecognizer::UNKNOWN_TYPE = "UKNOWN";
const std::string FileRecognizer::UNKNOWN_EXTENSION = "UNKNOWN";

std::map<std::string, std::string> FileRecognizer::mimeTypeToFileExtension_ =
    boost::assign::map_list_of
        ("application/octet-stream", "bin") //?
        ("application/pdf",          "pdf")
        ("application/postscript",   "eps")
        ("application/xml",          "xml")
        ("application/x-empty",      "") //?
        ("application/x-gzip",       "gz")
        ("image/gif",                "gif")
        ("image/jpeg",               "jpg")
        ("image/png",                "png")
        ("image/svg+xml",            "svg")
        ("image/tiff",               "tiff")
        ("image/vnd.djvu",           "djvu")
        ("model/vrml",               "wrl")
        ("text/html",                "html")
        ("text/plain",               "txt")
        ("text/rtf",                 "rtf");

FileRecognizer::FileRecognizer() {
    //initializeLibMagic();
}

FileRecognizer::~FileRecognizer() {
    //closeLibMagic();
}

void FileRecognizer::initializeLibMagic() {
#if HAVE_LIBMAGIC
    magicCookie_ = magic_open(MAGIC_MIME);

    if (magicCookie_ == NULL) {
        ERROR("Unable to initialize magic library");
    }
    if (magic_load(magicCookie_, NULL) != 0) {
        ERROR("Cannot load magic database: " << magic_error(magicCookie_));
    };
#endif
}

void FileRecognizer::closeLibMagic() {
#if HAVE_LIBMAGIC
    magic_close(magicCookie_);
#endif
}

#if HAVE_LIBMAGIC
std::string FileRecognizer::recognizeMimeType(const std::string & data) {
#else
std::string FileRecognizer::recognizeMimeType(const std::string & /*data*/) {
#endif
    std::string type = UNKNOWN_TYPE;

#if HAVE_LIBMAGIC
    initializeLibMagic();
    const char* magic = magic_buffer(magicCookie_, data.c_str(), data.length());

    if (!magic) {
        ERROR("magic is not defined when trying to recognize mime type!");
    }
    else {
        std::string magicFileInfo = magic;
        type = getMimeType_(magicFileInfo);

        DEBUG("magic filetype: [" << magicFileInfo << "], file type: [" << type << "]");
    }

    closeLibMagic();
#endif

    return type;
}

std::string FileRecognizer::getMimeType_(const std::string& magicFileInfo) {
    size_t sep = magicFileInfo.find_first_of('/');

    if (sep != std::string::npos) {
        return magicFileInfo.substr(0, sep);
    }

    return UNKNOWN_TYPE;
}

#if HAVE_LIBMAGIC
std::string FileRecognizer::recognizeFileExtension(const std::string & data) {
#else
std::string FileRecognizer::recognizeFileExtension(const std::string & /*data*/) {
#endif
    std::string extension = UNKNOWN_TYPE;

#if HAVE_LIBMAGIC
    initializeLibMagic();
    const char* magic = magic_buffer(magicCookie_, data.c_str(), data.length());

    if (!magic) {
        ERROR("magic is not defined when trying to get file extension!");
    }
    else {
        std::string magicFileInfo = magic;
        extension = getFileExtension_(magicFileInfo);

        DEBUG("magic filetype: [" << magicFileInfo << "], file extension: [" << extension << "]");
    }

    closeLibMagic();
#endif

    return extension;
}

std::string FileRecognizer::getFileExtension_(const std::string& magicFileInfo) {
    size_t sep = magicFileInfo.find_first_of(';');
    std::string magicFileType;

    if (sep != std::string::npos) {
        magicFileType = magicFileInfo.substr(0, sep);
    }

    if (mimeTypeToFileExtension_.find(magicFileType) != mimeTypeToFileExtension_.end()) {
        return mimeTypeToFileExtension_[magicFileType];
    }

    return UNKNOWN_EXTENSION;
}

#if HAVE_LIBMAGIC
void FileRecognizer::recognizeMimeTypeAndFileExtension(const std::string & data,
    std::string &type,
    std::string &extension) {
#else
void FileRecognizer::recognizeMimeTypeAndFileExtension(const std::string & /*data*/,
    std::string &type,
    std::string &extension) {
#endif

    type = UNKNOWN_TYPE;
    extension = UNKNOWN_EXTENSION;

#if HAVE_LIBMAGIC
    initializeLibMagic();
    const char* magic = magic_buffer(magicCookie_, data.c_str(), data.length());

    if (!magic) {
        ERROR("magic is not defined when trying to recognize mime type and file extension!");
    }
    else {
        std::string magicFileInfo = magic;
        type = getMimeType_(magicFileInfo);
        extension = getFileExtension_(magicFileInfo);
    }

    closeLibMagic();
#endif
}

