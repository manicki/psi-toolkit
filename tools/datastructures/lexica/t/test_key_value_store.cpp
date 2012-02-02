#include "tests.hpp"

#include "key_value_store.hpp"

#include <vector>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>

#include <stdio.h>

BOOST_AUTO_TEST_CASE(key_value_store_test) {
    KeyValueStore::Builder builder;

    builder.add("Poland", "Warsaw");
    builder.add("Finland", "Helsinki");
    builder.add("Italy", "Rome");
    builder.add("France", "Paris");

    boost::scoped_ptr<KeyValueStore> store(builder.build());

    BOOST_REQUIRE(store->get("Finland"));
    BOOST_CHECK_EQUAL(store->get("Finland").get(), std::string("Helsinki"));
    BOOST_REQUIRE(store->get("Poland"));
    BOOST_CHECK_EQUAL(store->get("Poland").get(), std::string("Warsaw"));
    BOOST_REQUIRE(store->get("Italy"));
    BOOST_CHECK_EQUAL(store->get("Italy"), std::string("Rome"));

    BOOST_CHECK(not store->get("Sweden"));
    BOOST_CHECK(not store->get(""));
    BOOST_CHECK(not store->get("Polanda"));
}

BOOST_AUTO_TEST_CASE(key_value_store_load_test) {
    std::vector<const char*> keys = boost::assign::list_of
        ("ź")
        ("bok")
        ("kota")
        ("źdźbło")
        ("亚当")
        ("")
        ("foo")
        ("bar")
        ("baz");

    std::vector<const char*> values = boost::assign::list_of
        ("123")
        ("")
        ("123")
        ("foo")
        ("bar")
        ("baaa")
        ("bok")
        ("źdźbło")
        ("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    BOOST_REQUIRE_EQUAL(keys.size(), values.size());

    char* tmpFile = tempnam(0, "test_key_value_store_bin");

    {
        KeyValueStore::Builder builder;

        for (size_t i = 0; i < keys.size(); ++i)
            builder.add(keys[i], values[i]);

        boost::scoped_ptr<KeyValueStore> store(builder.build());

        for (size_t i = 0; i < keys.size(); ++i) {
            boost::optional<std::string> valueFound = store->get(keys[i]);
            BOOST_REQUIRE(valueFound);
            BOOST_CHECK_EQUAL(valueFound.get(), values[i]);
        }

        std::FILE* mphf = std::fopen(tmpFile, "w");
        store->save(mphf);
        std::fclose(mphf);
    }

    {
        KeyValueStore loadedStore;

        std::FILE* mphf = std::fopen(tmpFile, "r");
        loadedStore.load(mphf);
        std::fclose(mphf);

        for (size_t i = 0; i < keys.size(); ++i) {
            boost::optional<std::string> valueFound = loadedStore.get(keys[i]);
            BOOST_REQUIRE(valueFound);
            BOOST_CHECK_EQUAL(valueFound.get(), values[i]);
        }
    }

    free(tmpFile);
}
