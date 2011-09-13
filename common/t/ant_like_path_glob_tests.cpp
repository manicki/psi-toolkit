#include "tests.hpp"

#include "ant_like_path_glob.hpp"

#include "config.h"

BOOST_AUTO_TEST_CASE( simple_ant_like_path_glob ) {
    AntLikePathGlob glob("foo/**/*.txt");

    std::set<boost::filesystem::path> matchedFiles;

    glob.allMatchingFiles(
        ROOT_DIR "common/t/ant_like_path_glob_tests_dir",
        matchedFiles);

    BOOST_REQUIRE_EQUAL(matchedFiles.size(), 4U);

    std::set<boost::filesystem::path>::const_iterator iter = matchedFiles.begin();

    BOOST_CHECK_EQUAL(*iter,
                      ROOT_DIR "common/t/ant_like_path_glob_tests_dir/foo/bar/baz/bar/bla.txt");
    ++iter;
    BOOST_CHECK_EQUAL(*iter,
                      ROOT_DIR "common/t/ant_like_path_glob_tests_dir/foo/bar/baz/test.txt");

    ++iter;
    BOOST_CHECK_EQUAL(*iter,
                      ROOT_DIR "common/t/ant_like_path_glob_tests_dir/foo/bar/baz/testB.txt");

    ++iter;
    BOOST_CHECK_EQUAL(*iter,
                      ROOT_DIR "common/t/ant_like_path_glob_tests_dir/foo/bla.txt");

}

BOOST_AUTO_TEST_CASE( single_file ) {
    AntLikePathGlob glob("foo/bar/baz/test.txt");

    std::set<boost::filesystem::path> matchedFiles;

    glob.allMatchingFiles(
        ROOT_DIR "common/t/ant_like_path_glob_tests_dir",
        matchedFiles);

    BOOST_REQUIRE_EQUAL(matchedFiles.size(), 1U);

    std::set<boost::filesystem::path>::const_iterator iter = matchedFiles.begin();

    BOOST_CHECK_EQUAL(*iter,
                      ROOT_DIR "common/t/ant_like_path_glob_tests_dir/foo/bar/baz/test.txt");

}

BOOST_AUTO_TEST_CASE( single_file_no_match ) {
    AntLikePathGlob glob("foo/bar/baz/test.gif");

    std::set<boost::filesystem::path> matchedFiles;

    glob.allMatchingFiles(
        ROOT_DIR "common/t/ant_like_path_glob_tests_dir",
        matchedFiles);


    BOOST_CHECK_EQUAL(matchedFiles.size(), 0U);
}

BOOST_AUTO_TEST_CASE( single_file_tricky ) {
    AntLikePathGlob glob("foo/bar/baz/test.txt/**");

    std::set<boost::filesystem::path> matchedFiles;

    glob.allMatchingFiles(
        ROOT_DIR "common/t/ant_like_path_glob_tests_dir",
        matchedFiles);

    BOOST_CHECK_EQUAL(matchedFiles.size(), 0U);
}

BOOST_AUTO_TEST_CASE( in_the_middle ) {
    AntLikePathGlob glob("**/bar/**");

    std::set<boost::filesystem::path> matchedFiles;

    glob.allMatchingFiles(
        ROOT_DIR "common/t/ant_like_path_glob_tests_dir",
        matchedFiles);

    BOOST_REQUIRE_EQUAL(matchedFiles.size(), 3U);

    std::set<boost::filesystem::path>::const_iterator iter = matchedFiles.begin();

    BOOST_CHECK_EQUAL(*iter,
                      ROOT_DIR "common/t/ant_like_path_glob_tests_dir/foo/bar/baz/bar/bla.txt");

    ++iter;
    BOOST_CHECK_EQUAL(*iter,
                      ROOT_DIR "common/t/ant_like_path_glob_tests_dir/foo/bar/baz/test.txt");

    ++iter;
    BOOST_CHECK_EQUAL(*iter,
                      ROOT_DIR "common/t/ant_like_path_glob_tests_dir/foo/bar/baz/testB.txt");

}
