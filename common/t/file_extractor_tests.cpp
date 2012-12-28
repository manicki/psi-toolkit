#include "tests.hpp"
#include "logging.hpp"

#include <fstream>
#include <boost/foreach.hpp>

#include "file_extractor.hpp"

BOOST_AUTO_TEST_SUITE( file_extractor )

BOOST_AUTO_TEST_CASE( get_file_list_from_zip_file_by_path ) {
    FileExtractor fileExtractor;

    std::set<std::string> files = fileExtractor.getFileListByPath(
        ROOT_DIR "common/t/files/example.zip");

    BOOST_CHECK( files.find("ala.txt") != files.end() );
    BOOST_CHECK( files.find("marysia.txt") != files.end() );
}
// FIXME
/*
BOOST_AUTO_TEST_CASE( extract_zip_file_by_path ) {
    FileExtractor fileExtractor;

    std::map<std::string, std::string> filesWithContent = fileExtractor.extractFilesByPath(
        ROOT_DIR "common/t/files/example.zip", "");

    std::pair<std::string, std::string> element;
    std::set<std::string> files;
    BOOST_FOREACH( element, filesWithContent ) {
        files.insert(element.first);
    }

    BOOST_CHECK( files.find("ala.txt") != files.end() );
    BOOST_CHECK( files.find("marysia.txt") != files.end() );

    BOOST_CHECK_EQUAL( filesWithContent["ala.txt"], "Ala ma kota." );
    BOOST_CHECK_EQUAL( filesWithContent["marysia.txt"], "Marysia ma rysia." );
}

BOOST_AUTO_TEST_CASE( extract_zip_file_from_data ) {
    FileExtractor fileExtractor;

    std::ifstream ifs(ROOT_DIR "common/t/files/example.zip");
    std::string compressedFile((std::istreambuf_iterator<char>(ifs)),
                               (std::istreambuf_iterator<char>()));

    WARN(compressedFile.size());

    std::map<std::string, std::string> filesWithContent = fileExtractor.extractFilesFromData(
        compressedFile, "");

    std::pair<std::string, std::string> element;
    std::set<std::string> files;
    BOOST_FOREACH( element, filesWithContent ) {
        files.insert(element.first);
    }

    BOOST_CHECK( files.find("ala.txt") != files.end() );
    BOOST_CHECK( files.find("marysia.txt") != files.end() );

    BOOST_CHECK_EQUAL( filesWithContent["ala.txt"], "Ala ma kota." );
    BOOST_CHECK_EQUAL( filesWithContent["marysia.txt"], "Marysia ma rysia." );
}
*/

BOOST_AUTO_TEST_SUITE_END()
