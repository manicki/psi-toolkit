#include <boost/assign.hpp>

#include "format_recognizer.hpp"

const std::vector<std::string> FormatRecognizer::HANDLED_FILETYPES = boost::assign::list_of
    ("txt")("bin");

FormatRecognizer::FormatRecognizer() { }

bool FormatRecognizer::isHandledFiletype(const std::string& filetype) {
    return (std::find(HANDLED_FILETYPES.begin(), HANDLED_FILETYPES.end(), filetype) !=
        HANDLED_FILETYPES.end());
}

std::string FormatRecognizer::recognize(const std::string& data, const std::string& filetype) {

    if (filetype == "bin") {
        if (hasDjvuFormatBeginning(data)) {
            return "djvu";
        }
    }

    return filetype;
}

bool FormatRecognizer::hasDjvuFormatBeginning(const std::string& data) {
    std::string djvuBeginning = "AT&TFORM";
    return ((data.compare(0, djvuBeginning.size(), djvuBeginning) == 0) ? true : false);
}
