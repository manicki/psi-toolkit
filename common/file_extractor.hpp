#ifndef FILE_EXTRACTOR_HDR
#define FILE_EXTRACTOR_HDR

#include <string>
#include <list>

#include "fex.h"

class FileExtractor {

public:
    FileExtractor();

    std::list<std::string> extract(
        const std::string &archive,
        const std::string &file);

    std::list<std::string> extractAllWithExtension(
        const std::string &archive,
        const std::string &extension);

private:
    void processFile_(fex_t* fex);

};

#endif
