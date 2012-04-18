#include <boost/assign.hpp>

#include "file_recognizer.hpp"
#include "logging.hpp"

const std::string FileRecognizer::UNKNOWN_TYPE = "UKNOWN";

std::map<std::string, std::string> FileRecognizer::magicFileTypeToFileExtension_ =
    boost::assign::map_list_of
        ("application/octet-stream", "bin") //?
        ("application/pdf", "pdf")
        ("application/postscript", "eps")
        ("application/xml", "xml")
        ("application/x-empty", "") //?
        ("application/x-gzip", "gz")
        ("image/gif", "gif")
        ("image/jpeg", "jpg")
        ("image/png", "png")
        ("image/svg+xml", "svg")
        ("image/tiff", "tiff")
        ("model/vrml", "wrl")
        ("text/html", "html")
        ("text/plain", "txt")
        ;

FileRecognizer::FileRecognizer() {
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

FileRecognizer::~FileRecognizer() {
#if HAVE_LIBMAGIC
    magic_close(magicCookie_);
#endif
}

#if HAVE_LIBMAGIC
std::string FileRecognizer::recognizeType(const std::string & data) {
#else
std::string FileRecognizer::recognizeType(const std::string & /*data*/) {
#endif
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

#if HAVE_LIBMAGIC
std::string FileRecognizer::recognizeExtension(const std::string & data) {
#else
std::string FileRecognizer::recognizeExtension(const std::string & /*data*/) {
#endif
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
