#include "file_extractor.hpp"
#include "logging.hpp"

#include <stdio.h>

FileExtractor::FileExtractor() {
};

std::list<std::string> FileExtractor::extract(
    const std::string &archive,
    const std::string &file) {

    std::list<std::string> fileContents;
    return fileContents;
};

std::list<std::string> FileExtractor::extractAllWithExtension(
    const std::string &archive,
    const std::string &extension) {

    fex_t* fex;
    fex_open(&fex, archive.c_str());

    while (!fex_done(fex)) {
        if (fex_has_extension(fex_name(fex), extension.c_str())) {
            processFile_(fex);
        }
        fex_next(fex);
    }

    std::list<std::string> fileContents;
    return fileContents;
};

void FileExtractor::processFile_(fex_t* fex) {
	const void* data;
	int size;
	int i;

	printf( "Processing %s\n", fex_name( fex ) );

	/* Get pointer to extracted data. Fex will automatically free this later. */
	fex_data( fex, &data );
	size = fex_size( fex );

	/* Print first 100 characters */
	for ( i = 0; i < size && i < 100; ++i )
		putchar( ((const char*) data) [i] );
	putchar( '\n' );
};
