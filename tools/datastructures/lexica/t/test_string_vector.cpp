#include "tests.hpp"

#include "string_vector.hpp"

#include <vector>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>

#include <stdio.h>

BOOST_AUTO_TEST_CASE(string_vector_test) {

    std::vector<const char*> wantedStrings = boost::assign::list_of
        ("Ala")
        ("ma")
        ("kota")
        ("źdźbło")
        ("亚当")
        ("")
        ("foo")
        ("bar")
        ("baz");

    char* tmpFile = tempnam(0, "test_string_vector_bin");

    {
        StringVector<> v;

        BOOST_CHECK_EQUAL(v.size(), 0);

        BOOST_FOREACH(const char* s, wantedStrings) {
            v.push_back(s);
        }

        BOOST_CHECK_EQUAL(v.size(), wantedStrings.size());

        for (size_t i = 0; i < wantedStrings.size(); ++i) {
            BOOST_CHECK_EQUAL(v[i].str(), std::string(wantedStrings[i]));
        }

        v.save(tmpFile);
    }

    {
        StringVector<> loadedV;

        loadedV.load(tmpFile);

        BOOST_CHECK_EQUAL(loadedV.size(), wantedStrings.size());

        for (size_t i = 0; i < wantedStrings.size(); ++i) {
            BOOST_CHECK_EQUAL(loadedV[i].str(), std::string(wantedStrings[i]));
        }
    }

    free(tmpFile);
}
