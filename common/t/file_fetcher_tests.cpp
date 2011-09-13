#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>

#include "../file_fetcher.hpp"
#include "../lang_specific_processor_file_fetcher.hpp"

#include "config.h"

BOOST_AUTO_TEST_CASE( file_fetcher_simple ) {
    FileFetcher fileFetcher;

    fileFetcher.addParam("BAZZZ", "baz");
    fileFetcher.addParam("FO", "foo");
    fileFetcher.addParam("MAIN", ROOT_DIR "common/t/ant_like_path_glob_tests_dir");

    std::vector<boost::filesystem::path> filesFound;

    std::string fileSpec = "%MAIN%/%FO%/**/%BAZZZ%/**/*.txt";

    fileFetcher.getFiles(fileSpec, std::back_inserter(filesFound));

    BOOST_REQUIRE_EQUAL(filesFound.size(), 3U);

    BOOST_CHECK_EQUAL(filesFound[0].string(),
                      "../common/t/ant_like_path_glob_tests_dir/foo/bar/baz/bar/bla.txt");

    BOOST_CHECK_EQUAL(filesFound[1].string(),
                      "../common/t/ant_like_path_glob_tests_dir/foo/bar/baz/test.txt");

    BOOST_CHECK_EQUAL(filesFound[2].string(),
                      "../common/t/ant_like_path_glob_tests_dir/foo/bar/baz/testB.txt");

    BOOST_CHECK_THROW(fileFetcher.getOneFile(fileSpec), FileFetcher::Exception);
}

BOOST_AUTO_TEST_CASE( file_fetcher_single_file ) {
    FileFetcher fileFetcher;

    fileFetcher.addParam(
        "MAIN",
        ROOT_DIR "common/t/ant_like_path_glob_tests_dir");
    fileFetcher.addParam("NAME", "bla");

    std::string fileSpec = "%MAIN%/*/%NAME%.txt";

    BOOST_CHECK_EQUAL(
        fileFetcher.getOneFile(fileSpec).string(),
        "../common/t/ant_like_path_glob_tests_dir/foo/bla.txt");
}

BOOST_AUTO_TEST_CASE( processor_fetcher ) {
    // real file must be used
    LangSpecificProcessorFileFetcher fileFetcher(
        ROOT_DIR "tools/tokenizers/tp/tp_basic_tokenizer_rule_set.cpp",
        "pl");

    fileFetcher.addParam(
        "MAIN",
        ROOT_DIR "common/t/ant_like_path_glob_tests_dir");
    fileFetcher.addParam("NAME", "bla");

    BOOST_CHECK_EQUAL(
        fileFetcher.getOneFile("%ITSDATA%/%LANG%/%LANG%.rgx"),
        "../tools/tokenizers/tp/data/pl/pl.rgx");

    std::vector<boost::filesystem::path> filesFound;

    fileFetcher.getFiles(
        "%ITSDATA%/**/%LANG%.rgx,%ITSDATA%/xx/xx.rgx,%MAIN%/*/%NAME%.txt",
        std::back_inserter(filesFound));

    BOOST_REQUIRE_EQUAL(filesFound.size(), 3U);

    BOOST_CHECK_EQUAL(
        filesFound[0].string(),
        "../common/t/ant_like_path_glob_tests_dir/foo/bla.txt");

    BOOST_CHECK_EQUAL(
        filesFound[1].string(),
        "../tools/tokenizers/tp/data/pl/pl.rgx");

    BOOST_CHECK_EQUAL(
        filesFound[2].string(),
        "../tools/tokenizers/tp/data/xx/xx.rgx");

}

BOOST_AUTO_TEST_CASE( exceptions ) {
    FileFetcher fileFetcher;

    std::vector<boost::filesystem::path> filesFound;

    BOOST_CHECK_THROW(
        fileFetcher.getFiles("%FOO%", std::back_inserter(filesFound)),
        FileFetcher::Exception);
}
