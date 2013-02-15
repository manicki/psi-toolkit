#include "tests.hpp"

#include <boost/assign.hpp>

#include "layer_tag_collection.hpp"
#include "layer_tag_manager.hpp"

BOOST_AUTO_TEST_SUITE( layer_tags )

BOOST_AUTO_TEST_CASE( tags_simple ) {
    LayerTagManager layer_tag_manager;

    LayerTagCollection tag_foo = layer_tag_manager.createSingletonTagCollection("foo");
    LayerTagCollection tag_bar = layer_tag_manager.createSingletonTagCollection("bar");

    BOOST_CHECK(tag_foo != tag_bar);

    LayerTagCollection tags_empty = createIntersection(tag_foo, tag_bar);

    BOOST_CHECK(tags_empty.isEmpty());

    LayerTagCollection tags_foo_bar = createUnion(tag_foo, tag_bar);

    BOOST_CHECK(!tags_foo_bar.isEmpty());
    BOOST_CHECK(tags_foo_bar.isNonempty());

    BOOST_CHECK(tag_foo < tags_foo_bar);
    BOOST_CHECK(tag_bar < tags_foo_bar);

    std::list<std::string> tag_list = layer_tag_manager.getTagNames(tags_foo_bar);
    BOOST_CHECK_EQUAL(tag_list.size(), 2U);
    BOOST_CHECK_EQUAL(tag_list.front(), "bar");
    BOOST_CHECK_EQUAL(tag_list.back(), "foo");
}

BOOST_AUTO_TEST_CASE( tags_names_list ) {
    LayerTagManager layer_tag_manager;

    std::list<std::string> namesList;
    namesList.push_back("foo");
    namesList.push_back("bar");

    LayerTagCollection tags = layer_tag_manager.createTagCollectionFromList(namesList);

    BOOST_CHECK(!tags.isEmpty());
    BOOST_CHECK(tags.isNonempty());

    std::list<std::string> tagList = layer_tag_manager.getTagNames(tags);
    BOOST_CHECK_EQUAL(tagList.size(), 2U);
    BOOST_CHECK_EQUAL(tagList.front(), "bar");
    BOOST_CHECK_EQUAL(tagList.back(), "foo");
}

BOOST_AUTO_TEST_CASE( tags_names_vector ) {
    LayerTagManager layer_tag_manager;

    std::vector<std::string> namesVector;
    namesVector.push_back("foo");
    namesVector.push_back("bar");

    LayerTagCollection tags = layer_tag_manager.createTagCollectionFromVector(namesVector);

    BOOST_CHECK(!tags.isEmpty());
    BOOST_CHECK(tags.isNonempty());

    std::list<std::string> tagList = layer_tag_manager.getTagNames(tags);
    BOOST_CHECK_EQUAL(tagList.size(), 2U);
    BOOST_CHECK_EQUAL(tagList.front(), "bar");
    BOOST_CHECK_EQUAL(tagList.back(), "foo");
}

BOOST_AUTO_TEST_CASE( tags_masks ) {
    LayerTagManager layer_tag_manager;

    std::list<std::string> listFoo;
    listFoo.push_back("foo");
    LayerTagCollection tagFoo = layer_tag_manager.createSingletonTagCollection("foo");

    std::list<std::string> listBar;
    listBar.push_back("bar");
    LayerTagCollection tagBar = layer_tag_manager.createSingletonTagCollection("bar");

    LayerTagCollection tagFooBar = layer_tag_manager.createTagCollectionFromList(
        boost::assign::list_of
        (std::string("foo"))
        (std::string("bar")));

    LayerTagMask maskFoo = layer_tag_manager.getMask(tagFoo);
    LayerTagMask maskFooList = layer_tag_manager.getMask(listFoo);
    LayerTagMask maskBar = layer_tag_manager.getMask(tagBar);
    std::list<std::string> maskFooBarAsStrings =
        boost::assign::list_of
        (std::string("foo"))
        (std::string("bar"));
    LayerTagMask maskFooBar = layer_tag_manager.getMask(maskFooBarAsStrings);
    LayerTagMask maskFooBarList = layer_tag_manager.getMaskFromList(
        boost::assign::list_of
        (std::string("foo"))
        (std::string("bar")));
    LayerTagMask maskAny = layer_tag_manager.anyTag();

    BOOST_CHECK(maskFoo.isSome());
    BOOST_CHECK(maskFooList.isSome());
    BOOST_CHECK(maskBar.isSome());
    BOOST_CHECK(matches(tagFoo, maskFoo));
    BOOST_CHECK(matches(tagFoo, maskFooList));
    BOOST_CHECK(matches(tagFoo, maskAny));
    BOOST_CHECK(!matches(tagBar, maskFoo));
    BOOST_CHECK(!matches(tagBar, maskFooList));
    BOOST_CHECK(!matches(tagBar, maskFooBar));
    BOOST_CHECK(!matches(tagBar, maskFooBarList));
    BOOST_CHECK(!matches(tagFoo, maskFooBar));
    BOOST_CHECK(!matches(tagFoo, maskFooBarList));
    BOOST_CHECK(matches(tagFooBar, maskFoo));
    BOOST_CHECK(matches(tagFooBar, maskBar));
    BOOST_CHECK(matches(tagFooBar, maskFooBar));
    BOOST_CHECK(matches(tagFooBar, maskFooBarList));
}

BOOST_AUTO_TEST_CASE( planes ) {
    LayerTagManager layerTagManager;
    LayerTagCollection tagsFoo(layerTagManager.createSingletonTagCollection("foo"));
    LayerTagCollection tagsBar(layerTagManager.createSingletonTagCollection("bar"));
    LayerTagCollection tagsFooP(layerTagManager.createSingletonTagCollection("!foo"));
    LayerTagCollection tagsBarP(layerTagManager.createSingletonTagCollection("!bar"));
    BOOST_CHECK(layerTagManager.areInTheSamePlane(tagsFoo, tagsBar));
    BOOST_CHECK(!layerTagManager.areInTheSamePlane(tagsFooP, tagsBarP));

    {
        std::list<std::string> namesList1;
        namesList1.push_back("foo");
        namesList1.push_back("!boo");
        LayerTagCollection tags1 = layerTagManager.createTagCollectionFromList(namesList1);
        std::list<std::string> namesList2;
        namesList2.push_back("bar");
        namesList2.push_back("!boo");
        LayerTagCollection tags2 = layerTagManager.createTagCollectionFromList(namesList2);
        BOOST_CHECK(layerTagManager.areInTheSamePlane(tags1, tags2));
    }

    {
        std::list<std::string> namesList1;
        namesList1.push_back("!foo");
        namesList1.push_back("boo");
        LayerTagCollection tags1 = layerTagManager.createTagCollectionFromList(namesList1);
        std::list<std::string> namesList2;
        namesList2.push_back("!bar");
        namesList2.push_back("boo");
        LayerTagCollection tags2 = layerTagManager.createTagCollectionFromList(namesList2);
        BOOST_CHECK(!layerTagManager.areInTheSamePlane(tags1, tags2));
    }
}

BOOST_AUTO_TEST_SUITE_END()
