#include "file_fetcher.hpp"

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
};


