#include "file_fetcher.hpp"
#include "lang_specific_processor_file_fetcher.hpp"

#include "config.h"

class FileFetcherTests : public CxxTest::TestSuite
{
public:
    void testSimple() {
        FileFetcher fileFetcher;

        fileFetcher.addParam("BAZZZ", "baz");
        fileFetcher.addParam("FO", "foo");
        fileFetcher.addParam("MAIN", ROOT_DIR "common/t/ant_like_path_glob_tests_dir");

        std::vector<boost::filesystem::path> filesFound;

        std::string fileSpec = "%MAIN%/%FO%/**/%BAZZZ%/**/*.txt";

        fileFetcher.getFiles(fileSpec, std::back_inserter(filesFound));

        TS_ASSERT_EQUALS(filesFound.size(), 3U);

        TS_ASSERT_EQUALS(filesFound[0].string(),
                         "../common/t/ant_like_path_glob_tests_dir/foo/bar/baz/bar/bla.txt");

        TS_ASSERT_EQUALS(filesFound[1].string(),
                         "../common/t/ant_like_path_glob_tests_dir/foo/bar/baz/test.txt");

        TS_ASSERT_EQUALS(filesFound[2].string(),
                         "../common/t/ant_like_path_glob_tests_dir/foo/bar/baz/testB.txt");

        TS_ASSERT_THROWS(fileFetcher.getOneFile(fileSpec), FileFetcher::Exception);
    }

    void testSingleFile() {
        FileFetcher fileFetcher;

        fileFetcher.addParam(
            "MAIN",
            ROOT_DIR "common/t/ant_like_path_glob_tests_dir");
        fileFetcher.addParam("NAME", "bla");

        std::string fileSpec = "%MAIN%/*/%NAME%.txt";

        TS_ASSERT_EQUALS(
            fileFetcher.getOneFile(fileSpec).string(),
            "../common/t/ant_like_path_glob_tests_dir/foo/bla.txt");
    }

    void testProcessorFetcher() {
        // real file must be used
        LangSpecificProcessorFileFetcher fileFetcher(
            ROOT_DIR "tools/tokenizers/tp/tp_basic_tokenizer_rule_set.cpp",
            "pl");

        fileFetcher.addParam(
            "MAIN",
            ROOT_DIR "common/t/ant_like_path_glob_tests_dir");
        fileFetcher.addParam("NAME", "bla");

        TS_ASSERT_EQUALS(
            fileFetcher.getOneFile("%ITSDATA%/%LANG%/%LANG%.rgx"),
            "../tools/tokenizers/tp/data/pl/pl.rgx");

        std::vector<boost::filesystem::path> filesFound;

        fileFetcher.getFiles(
            "%ITSDATA%/**/%LANG%.rgx,%ITSDATA%/xx/xx.rgx,%MAIN%/*/%NAME%.txt",
            std::back_inserter(filesFound));

        TS_ASSERT_EQUALS(filesFound.size(), 3U);

        TS_ASSERT_EQUALS(
            filesFound[0].string(),
            "../common/t/ant_like_path_glob_tests_dir/foo/bla.txt");

        TS_ASSERT_EQUALS(
            filesFound[1].string(),
            "../tools/tokenizers/tp/data/pl/pl.rgx");

        TS_ASSERT_EQUALS(
            filesFound[2].string(),
            "../tools/tokenizers/tp/data/xx/xx.rgx");

    }

    void testExceptions() {
        FileFetcher fileFetcher;

        std::vector<boost::filesystem::path> filesFound;

        TS_ASSERT_THROWS(
            fileFetcher.getFiles("%FOO%", std::back_inserter(filesFound)),
            FileFetcher::Exception);
    }
};


