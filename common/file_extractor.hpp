#ifndef FILE_EXTRACTOR_HDR
#define FILE_EXTRACTOR_HDR

#include <string>
#include <set>
#include <map>

#include "fex.h"

class FileExtractor {

public:
    FileExtractor();

    std::set<std::string> getFileList(const std::string &archivePath);
    std::set<std::string> getFileListFromData(const std::string &archiveData);

/*
    std::map<std::string, std::stringstream> extractAll(const std::string &archivePath);
    std::map<std::string, std::stringstream> extractAll(std::istream archiveData);

    std::map<std::string, std::stringstream> extractAllWithExtension(
        const std::string &archivePath,
        const std::string &extension);
    std::map<std::string, std::stringstream> extractAllWithExtension(
        std::istream archive,
        const std::string &extension);
*/

private:
    std::string storeToTempFile_(const std::string &archive);

    bool handleError_(fex_err_t error);

    //void processFile_(fex_t* fex);

    static const std::string PATH_FOR_TEMP_FILES;

};

#endif
