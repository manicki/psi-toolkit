#include "tests.hpp"

#include "layer_tag_collection.hpp"
#include "layer_tag_manager.hpp"

BOOST_AUTO_TEST_SUITE( layer_tags )

BOOST_AUTO_TEST_CASE( tags_simple ) {
    LayerTagManager layer_tag_manager;

    LayerTagCollection tag_foo = layer_tag_manager.createSingletonTagCollection("foo");
    LayerTagCollection tag_bar = layer_tag_manager.createSingletonTagCollection("bar");

    LayerTagCollection tags_empty = createIntersection(tag_foo, tag_bar);

    BOOST_CHECK(tags_empty.isEmpty());

    LayerTagCollection tags_foo_bar = createUnion(tag_foo, tag_bar);

    BOOST_CHECK(!tags_foo_bar.isEmpty());
    BOOST_CHECK(tags_foo_bar.isNonempty());

    std::list<std::string> tag_list = layer_tag_manager.getTagNames(tags_foo_bar);
    BOOST_CHECK_EQUAL(tag_list.size(), 2U);
    BOOST_CHECK_EQUAL(tag_list.front(), "bar");
    BOOST_CHECK_EQUAL(tag_list.back(), "foo");
}

BOOST_AUTO_TEST_SUITE_END()
