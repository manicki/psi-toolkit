#include <boost/filesystem.hpp>
#include <cstdio>
#include <fstream>
#include <cstring>

#include "file_extractor.hpp"
#include "md5.hpp"
#include "regexp.hpp"
#include "logging.hpp"

const std::string FileExtractor::PATH_FOR_TEMP_FILES = "/tmp/psi-toolkit";

FileExtractor::FileExtractor() {
    if (!boost::filesystem::exists(PATH_FOR_TEMP_FILES)) {
        if (!boost::filesystem::create_directory(PATH_FOR_TEMP_FILES)) {
            ERROR("temporary directory can not be created!");
        }
    }
};

std::set<std::string> FileExtractor::getFileListByPath(const std::string &archivePath) {
    std::set<std::string> files;

    fex_t* fex;
    handleError_( fex_open(&fex, archivePath.c_str()) );

    while (!fex_done(fex)) {
        std::string foundFile = fex_name(fex);

        files.insert(foundFile);
        DEBUG("found in archive: " << foundFile);

        handleError_( fex_next(fex) );
    }

    fex_close(fex);
    fex = NULL;

    return files;
}

std::set<std::string> FileExtractor::getFileListFromData(const std::string &archiveData) {
    return getFileListByPath(storeToTempFile_(archiveData));
}

std::map<std::string, std::string> FileExtractor::extractFilesByPath(
    const std::string &archivePath,
    const std::string &regexp) {

    std::map<std::string, std::string> files;

    fex_t* fex;
    handleError_( fex_open(&fex, archivePath.c_str()) );

    while (!fex_done(fex)) {
        if (regexp.empty() || RegExp::FullMatch(fex_name(fex), PerlRegExp(regexp))) {
            DEBUG("extracting file " << fex_name(fex));
            files.insert(std::pair<std::string, std::string>(
                fex_name(fex),
                processFile_(fex)
            ));
        }

        handleError_( fex_next(fex) );
    }

    fex_close(fex);
    fex = NULL;

    return files;
}

std::map<std::string, std::string> FileExtractor::extractFilesFromData(
    const std::string &archiveData,
    const std::string &regexp) {

    return extractFilesByPath(storeToTempFile_(archiveData), regexp);
}

bool FileExtractor::handleError_(fex_err_t error) {
    if (error != NULL) {
        throw FileExtractionException(fex_err_str(error));
    }

    return true;
}

std::string FileExtractor::storeToTempFile_(const std::string &archive) {
    std::string fileName = PATH_FOR_TEMP_FILES + "/" + md5(archive);

    if (boost::filesystem::exists(fileName)) {
        INFO("temporary file " << fileName << " exists");
        return fileName;
    }

    std::ofstream tempFile(fileName.c_str(), std::ios::out | std::ios::binary);

    if (tempFile) {
        INFO("creating temporary file " << fileName);
        tempFile.write(archive.c_str(), archive.size());
    }
    else {
        ERROR("temporary file can not be created!");
    }

    tempFile.close();
    return fileName;
}

std::string FileExtractor::processFile_(fex_t* fex) {
	const void* data;
    std::string content;

	handleError_( fex_data(fex, &data) );
	int size = fex_size(fex);

	for (int i = 0; i < size; ++i) {
		content += ((const char*) data)[i];
    }

    return content;
};
