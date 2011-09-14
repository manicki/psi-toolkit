
/*!development,testing/100
To run all tests, use `test_runner`:
    :::cltu
    $ tests/test_runner
    ...
    Running ... test cases...
    ...
    *** No errors detected

You can test only a selected test suite with the `-t` option:
    :::cltu
    $ tests/test_runner -t file_fetcher
    Running 4 test cases...
    ...
    *** No errors detected

Or you can run a single test case of a given test suite, e.g.:
    :::cltu
    $ tests/test_runner -t file_fetcher/simple
    Running 1 test case...
    ...
    *** No errors detected
*/

#define BOOST_TEST_MODULE Main
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


