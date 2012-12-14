#ifndef FILE_EXTRACTOR_HDR
#define FILE_EXTRACTOR_HDR

#include <string>
#include <set>
#include <map>

#include "fex.h"

class FileExtractor {

public:
    FileExtractor();

    std::set<std::string> getFileListByPath(const std::string &archivePath);
    std::set<std::string> getFileListFromData(const std::string &archiveData);

    std::map<std::string, std::string> extractFilesByPath(
        const std::string &archivePath,
        const std::string &regexp);
    std::map<std::string, std::string> extractFilesFromData(
        const std::string &archiveData,
        const std::string &regexp);

private:
    std::string storeToTempFile_(const std::string &archive);

    bool handleError_(fex_err_t error);
    std::string processFile_(fex_t* fex);

    static const std::string PATH_FOR_TEMP_FILES;

};

#endif
