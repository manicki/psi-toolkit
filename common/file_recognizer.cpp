#include <boost/assign.hpp>
#include <boost/foreach.hpp>

#include "file_recognizer.hpp"
#include "file_extractor.hpp"
#include "logging.hpp"
#include "regexp.hpp"

const std::string FileRecognizer::UNKNOWN_TYPE = "UKNOWN";
const std::string FileRecognizer::UNKNOWN_EXTENSION = "UNKNOWN";

//const std::vector<std::string> FileRecognizer::FILETYPES_FOR_FORMAT_RECOGNITION =
//    boost::assign::list_of("txt")("bin")("zip");

std::map<std::string, std::string> FileRecognizer::mimeTypeToFileExtension_ =
    boost::assign::map_list_of
        ("application/octet-stream", "bin") //?
        ("application/pdf",          "pdf")
        ("application/postscript",   "eps")
        ("application/xml",          "xml")
        ("application/x-empty",      "") //?
        ("application/x-gzip",       "gz")
        ("application/zip",          "zip")
        ("image/gif",                "gif")
        ("image/jpeg",               "jpg")
        ("image/png",                "png")
        ("image/svg+xml",            "svg")
        ("image/tiff",               "tiff")
        ("image/vnd.djvu",           "djvu")
        ("image/x.djvu",             "djvu")
        ("model/vrml",               "wrl")
        ("text/html",                "html")
        ("text/plain",               "txt")
        ("text/rtf",                 "rtf");

FileRecognizer::FileRecognizer() { }

FileRecognizer::~FileRecognizer() { }

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
        extension = recognizeFileFormat_(data.c_str(), extension);

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

std::string FileRecognizer::recognizeFileFormat_(
    const std::string& data,
    const std::string& filetype) {

    if (filetype == "bin") {
        if (hasDjvuFormatBeginning_(data)) {
            return "djvu";
        }
    }
    if (filetype == "txt") {
        if (looksLikePsiFormat_(data)) {
            return "psi";
        }
        if (looksLikeUTTFormat_(data)) {
            return "utt";
        }
    }

    return filetype;
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

bool FileRecognizer::hasDjvuFormatBeginning_(const std::string& data) {
    std::string djvuBeginning = "AT&TFORM";
    return ((data.compare(0, djvuBeginning.size(), djvuBeginning) == 0) ? true : false);
}

bool FileRecognizer::looksLikePsiFormat_(const std::string& text) {
    std::string arg;
    return RegExp::FullMatch(text, PerlRegExp("(01 0000 [0-9*][0-9@])(.|\n|\r)*"), &arg);
}

bool FileRecognizer::looksLikeUTTFormat_(const std::string& text) {
    std::string arg;
    return RegExp::FullMatch(text, PerlRegExp("(0000 00 [A-Z])(.|\n|\r)*"), &arg);
}

std::string FileRecognizer::recognizeCompressedFileFormat(const std::string &data) {
    // FIXME

    FileExtractor fileExtractor;
    std::list<std::string> extractedFiles = fileExtractor.getFileListFromData(data);

    BOOST_FOREACH(std::string fileName, extractedFiles) {
        DEBUG("extracted file: " << fileName);
    }

    return "docx";
}

