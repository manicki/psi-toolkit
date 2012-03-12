#include <boost/assign.hpp>

#include "file_recognizer.hpp"
#include "logging.hpp"

const std::string FileRecognizer::UNKNOWN_TYPE = "UKNOWN";

std::map<std::string, std::string> FileRecognizer::magicFileTypeToFileExtension_ =
    boost::assign::map_list_of
        ("text/plain", "txt")
        ("application/pdf", "pdf")
        ("application/postscript", "ps")
        ("image/svg+xml", "svg")
        ("image/jpeg", "jpg")
        ("image/jpg", "jpg")
        ("image/gif", "gif")
        ("image/png", "png")
        ("image/tiff", "tiff");

FileRecognizer::FileRecognizer() : magicCookie_(magic_open(MAGIC_MIME)) {
#if HAVE_LIBMAGIC
    if (magicCookie_ == NULL) {
        ERROR("Unable to initialize magic library");
    }
    if (magic_load(magicCookie_, NULL) != 0) {
        ERROR("Cannot load magic database: " << magic_error(magicCookie_));
    };
#endif
    SET_LOGGING_LEVEL("INFO"); //FIXME
}

FileRecognizer::~FileRecognizer() {
#if HAVE_LIBMAGIC
    magic_close(magicCookie_);
#endif
}

std::string FileRecognizer::recognizeType(const std::string & data) {
    std::string type = UNKNOWN_TYPE;

#if HAVE_LIBMAGIC
    std::string magicFileInfo = magic_buffer(magicCookie_, data.c_str(), data.length());
    type = getFileType_(magicFileInfo);

    INFO("magic filetype: [" << magicFileInfo << "], file type: [" << type << "]");
#endif

    return type;
}
std::string FileRecognizer::getFileType_(const std::string& magicFileInfo) {
    size_t sep = magicFileInfo.find_first_of('/');

    if (sep != std::string::npos) {
        return magicFileInfo.substr(0, sep);
    }

    return UNKNOWN_TYPE;
}

std::string FileRecognizer::recognizeExtension(const std::string & data) {
    std::string extension = UNKNOWN_TYPE;

#if HAVE_LIBMAGIC
    std::string magicFileInfo = magic_buffer(magicCookie_, data.c_str(), data.length());
    extension = getFileExtension_(magicFileInfo);

    INFO("magic filetype: [" << magicFileInfo << "], file extension: [" << extension << "]");
#endif

    return extension;
}
std::string FileRecognizer::getFileExtension_(const std::string& magicFileInfo) {
    size_t sep = magicFileInfo.find_first_of(';');
    std::string magicFileType;

    if (sep != std::string::npos) {
        magicFileType = magicFileInfo.substr(0, sep);
    }

    if (magicFileTypeToFileExtension_.find(magicFileType) != magicFileTypeToFileExtension_.end()) {
        return magicFileTypeToFileExtension_[magicFileType];
    }

    return UNKNOWN_TYPE;
}
