

#include "layer_tag_collection.hpp"
#include "layer_tag_manager.hpp"

class rules_tests : public CxxTest::TestSuite
{
public:
    void test_simple()
	{
            LayerTagManager layer_tag_manager;

            LayerTagCollection tag_foo = layer_tag_manager.createSingletonTagCollection("foo");
            LayerTagCollection tag_bar = layer_tag_manager.createSingletonTagCollection("bar");

            LayerTagCollection tags_foo_bar = createUnion(tag_foo, tag_bar);

            TS_ASSERT( ! tags_foo_bar.isEmpty() );

            std::list<std::string> tag_list = layer_tag_manager.getTagNames(tags_foo_bar);
            TS_ASSERT_EQUALS(tag_list.size(), 2);
            TS_ASSERT_EQUALS(tag_list.front(), "bar");
            TS_ASSERT_EQUALS(tag_list.back(), "foo");
        }

};
