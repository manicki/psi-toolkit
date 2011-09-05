#include "ant_like_path_glob.hpp"

class AntLikePathGlobTests : public CxxTest::TestSuite
{
public:
    void testSimple() {
        AntLikePathGlob glob("foo/**/*.txt");

        std::set<boost::filesystem::path> matchedFiles;

        glob.allMatchingFiles(
            ROOT_DIR "common/t/ant_like_path_glob_tests_dir",
            matchedFiles);

        TS_ASSERT_EQUALS(matchedFiles.size(), 4U);

        std::set<boost::filesystem::path>::const_iterator iter = matchedFiles.begin();

        TS_ASSERT_EQUALS(*iter,
                         ROOT_DIR "common/t/ant_like_path_glob_tests_dir/foo/bar/baz/bar/bla.txt");
        ++iter;
        TS_ASSERT_EQUALS(*iter,
                         ROOT_DIR "common/t/ant_like_path_glob_tests_dir/foo/bar/baz/test.txt");

        ++iter;
        TS_ASSERT_EQUALS(*iter,
                         ROOT_DIR "common/t/ant_like_path_glob_tests_dir/foo/bar/baz/testB.txt");

        ++iter;
        TS_ASSERT_EQUALS(*iter,
                         ROOT_DIR "common/t/ant_like_path_glob_tests_dir/foo/bla.txt");

    }

    void testSingleFile() {
        AntLikePathGlob glob("foo/bar/baz/test.txt");

        std::set<boost::filesystem::path> matchedFiles;

        glob.allMatchingFiles(
            ROOT_DIR "common/t/ant_like_path_glob_tests_dir",
            matchedFiles);

        TS_ASSERT_EQUALS(matchedFiles.size(), 1U);

        std::set<boost::filesystem::path>::const_iterator iter = matchedFiles.begin();

        TS_ASSERT_EQUALS(*iter,
                         ROOT_DIR "common/t/ant_like_path_glob_tests_dir/foo/bar/baz/test.txt");

    }

    void testSingleFileNoMatch() {
        AntLikePathGlob glob("foo/bar/baz/test.gif");

        std::set<boost::filesystem::path> matchedFiles;

        glob.allMatchingFiles(
            ROOT_DIR "common/t/ant_like_path_glob_tests_dir",
            matchedFiles);


        TS_ASSERT_EQUALS(matchedFiles.size(), 0U);
    }

    void testSingleFileTricky() {
        AntLikePathGlob glob("foo/bar/baz/test.txt/**");

        std::set<boost::filesystem::path> matchedFiles;

        glob.allMatchingFiles(
            ROOT_DIR "common/t/ant_like_path_glob_tests_dir",
            matchedFiles);

        TS_ASSERT_EQUALS(matchedFiles.size(), 0U);
    }

    void testInTheMiddle() {
        AntLikePathGlob glob("**/bar/**");

        std::set<boost::filesystem::path> matchedFiles;

        glob.allMatchingFiles(
            ROOT_DIR "common/t/ant_like_path_glob_tests_dir",
            matchedFiles);

        TS_ASSERT_EQUALS(matchedFiles.size(), 3U);

        std::set<boost::filesystem::path>::const_iterator iter = matchedFiles.begin();

        TS_ASSERT_EQUALS(*iter,
                         ROOT_DIR "common/t/ant_like_path_glob_tests_dir/foo/bar/baz/bar/bla.txt");

        ++iter;
        TS_ASSERT_EQUALS(*iter,
                         ROOT_DIR "common/t/ant_like_path_glob_tests_dir/foo/bar/baz/test.txt");

        ++iter;
        TS_ASSERT_EQUALS(*iter,
                         ROOT_DIR "common/t/ant_like_path_glob_tests_dir/foo/bar/baz/testB.txt");

    }
};
