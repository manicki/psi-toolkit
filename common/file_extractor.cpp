#include <boost/filesystem.hpp>
#include <cstdio>
#include <fstream>

#include "file_extractor.hpp"
#include "md5.hpp"
#include "logging.hpp"

const std::string FileExtractor::PATH_FOR_TEMP_FILES = "/tmp/psi-toolkit";

FileExtractor::FileExtractor() {
    if (!boost::filesystem::exists(PATH_FOR_TEMP_FILES)) {
        if (!boost::filesystem::create_directory(PATH_FOR_TEMP_FILES)) {
            ERROR("temporary directory can not be created!");
        }
    }
};

std::set<std::string> FileExtractor::getFileList(const std::string &archivePath) {
    std::set<std::string> files;

    //FIXME: obsluga bledow

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
    return getFileList(storeToTempFile_(archiveData));
}

/*
std::map<std::string, std::stringstream> FileExtractor::extractAll(
    const std::string &archivePath) {

    std::map<std::string, std::stringsteam> files;
    return files;
}

std::map<std::string, std::stringstream> FileExtractor::extractAllWithExtension(
    const std::string &archivePath,
    const std::string &extension) {

    fex_t* fex;
    fex_open(&fex, archive.c_str());

    while (!fex_done(fex)) {
        if (fex_has_extension(fex_name(fex), extension.c_str())) {
            processFile_(fex);
        }
        fex_next(fex);
    }

    fex_close(fex);
    fex = NULL;

    std::map<std::string, std::stringsteam> files;
    return files;
}
*/

bool FileExtractor::handleError_(fex_err_t error) {
    if (error != NULL) {
        ERROR("exception from fex: " << fex_err_str(error));
        return false;
    }

    return true;
}

std::string FileExtractor::storeToTempFile_(const std::string &archive) {
    std::string fileName = PATH_FOR_TEMP_FILES + "/" + md5(archive);

    if (boost::filesystem::exists(fileName)) {
        INFO("temporary file " << fileName << " exists");
        return fileName;
    }

    std::ofstream tempFile(fileName.c_str());

    if (tempFile) {
        INFO("creating temporary file " << fileName);
        tempFile << archive;
    }
    else {
        ERROR("temporary file can not be created!");
    }

    tempFile.close();
    return fileName;
}

/*
void FileExtractor::processFile_(fex_t* fex) {
	const void* data;
	int size;
	int i;

	printf( "Processing %s\n", fex_name( fex ) );

	fex_data( fex, &data );
	size = fex_size( fex );

	for ( i = 0; i < size; ++i )
		putchar( ((const char*) data) [i] );
	putchar( '\n' );
};
*/
