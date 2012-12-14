#include "file_extractor.hpp"
#include "logging.hpp"

#include <stdio.h>
#include <fstream>

const std::string FileExtractor::PATH_FOR_TEMP_FILES = "/tmp/psi-toolkit";

FileExtractor::FileExtractor() { };

std::set<std::string> FileExtractor::getFileList(const std::string &archivePath) {
    std::set<std::string> files;

    //FIXME: obsluga bledow

    fex_t* fex;
    fex_open(&fex, archivePath.c_str());

    while (!fex_done(fex)) {
        std::string foundFile = fex_name(fex);

        files.insert(foundFile);
        DEBUG("found in archive: " << foundFile);

        fex_next(fex);
    }

    fex_close(fex);
    fex = NULL;

    return files;
}

std::set<std::string> FileExtractor::getFileListFromData(const std::string &archive) {
    return getFileList(storeToTempFile_(archive));
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

std::string FileExtractor::storeToTempFile_(const std::string &archive) {
    //FIXME: nazwa pliku generowana z MD5
    std::string tempName = "input1234";
    std::ofstream tempFile(tempName.c_str());

    if (tempFile) {
        tempFile << archive;
    }
    else {
        WARN("temporary file can not be created!");
    }

    tempFile.close();
    return tempName;
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
