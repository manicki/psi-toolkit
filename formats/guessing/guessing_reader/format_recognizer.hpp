#ifndef FORMAT_RECOGNIZER_HDR
#define FORMAT_RECOGNIZER_HDR

#include <string>

#include "logging.hpp"

class FormatRecognizer {

public:

    FormatRecognizer();

    std::string recognize(const std::string& data, const std::string& filetype);
    bool isHandledFiletype(const std::string&);

private:

    static const std::vector<std::string> HANDLED_FILETYPES;

    bool hasDjvuFormatBeginning(const std::string&);

};

#endif
