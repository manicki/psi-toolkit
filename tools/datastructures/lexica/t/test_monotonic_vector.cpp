#include "tests.hpp"

#include "monotonic_vector.hpp"

#include <vector>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>

#include <stdio.h>

BOOST_AUTO_TEST_CASE(monotonic_vector_test) {
    std::vector<size_t> wantedNumbers = boost::assign::list_of
        (0)
        (1)
        (89)
        (110)
        (230)
        (10000)
        (10001)
        (100000)
        (100255)
        (200001)
        (200256);

    char* tmpFile = tempnam(0, "test_monotonic_vector_bin");

    {
        MonotonicVector<> v;

        BOOST_CHECK_EQUAL(v.size(), 0);

        BOOST_FOREACH(size_t n, wantedNumbers) {
            v.push_back(n);
        }

        BOOST_CHECK_EQUAL(v.size(), wantedNumbers.size());

        for (size_t i = 0; i < wantedNumbers.size(); ++i)
            BOOST_CHECK_EQUAL(v[i], wantedNumbers.at(i));

        std::FILE* mphf = std::fopen(tmpFile, "w");
        v.save(mphf);
        std::fclose(mphf);
    }

    {
        MonotonicVector<> loadedV;

        BOOST_CHECK_EQUAL(loadedV.size(), 0);

        std::FILE* mphf = std::fopen(tmpFile, "r");
        loadedV.load(mphf);
        std::fclose(mphf);

        BOOST_CHECK_EQUAL(loadedV.size(), wantedNumbers.size());

        for (size_t i = 0; i < wantedNumbers.size(); ++i)
            BOOST_CHECK_EQUAL(loadedV[i], wantedNumbers.at(i));
    }

    free(tmpFile);
}
