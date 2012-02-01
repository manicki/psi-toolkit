#include "tests.hpp"

#include "perfect_hash_index.hpp"

#include <list>
#include <map>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

BOOST_AUTO_TEST_CASE(perfect_hash_index_test) {
    std::list<const char*> wantedStrings = boost::assign::list_of
        ("Ala")
        ("ma")
        ("kota")
        ("źdźbło")
        ("亚当")
        ("")
        ("foo")
        ("bar")
        ("baz");

    std::list<const char*> unwantedStrings = boost::assign::list_of
        ("Alaxxx")
        ("ź")
        ("亚")
        ("ssssssssssss")
        ("fook");

    std::map<std::string, size_t> checkMap;
    size_t hashSize;

    boost::filesystem::path tmpFile =
        boost::filesystem::temp_directory_path() / "test_perfect_hash_index.bin";

    {
        PerfectHashIndex hash;

        BOOST_FOREACH(const char* s, wantedStrings) {
            hash.addKey(s);
        }

        hash.create();

        hashSize = hash.getSize();
        BOOST_CHECK_EQUAL(hashSize, wantedStrings.size());
        BOOST_CHECK(hashSize > 0);

        BOOST_FOREACH(const char* s, wantedStrings) {
            BOOST_CHECK(hash.getHash(s) < hashSize);
            checkMap[std::string(s)] = hash.getHash(s);
        }


        BOOST_FOREACH(const char* s, unwantedStrings) {
            BOOST_CHECK_EQUAL(hash.getHash(s), hashSize);
        }

        hash.save(tmpFile.string());
    }

    {
        PerfectHashIndex loadedHash;
        loadedHash.load(tmpFile.string());

        BOOST_CHECK_EQUAL(loadedHash.getSize(), hashSize);

        BOOST_FOREACH(const char* s, wantedStrings) {
            BOOST_CHECK_EQUAL(loadedHash.getHash(s), checkMap[std::string(s)]);
        }

        BOOST_FOREACH(const char* s, unwantedStrings) {
            BOOST_CHECK_EQUAL(loadedHash.getHash(s), hashSize);
        }
    }
}
