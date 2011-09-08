#include "object_cache.hpp"
#include "xml_property_tree.hpp"

class ObjectCacheTests : public CxxTest::TestSuite {

public:

    void testSimple() {
        boost::shared_ptr<XmlPropertyTree> objA1
            = ObjectCache::getInstance().getObject<XmlPropertyTree>(
            ROOT_DIR "common/t/simple.xml");

        boost::shared_ptr<XmlPropertyTree> objA2
            = ObjectCache::getInstance().getObject<XmlPropertyTree>(
            ROOT_DIR "common/t/simple.xml");

        TS_ASSERT_THROWS_ANYTHING(
            ObjectCache::getInstance().getObject<XmlPropertyTree>(
                ROOT_DIR "common/t/nonexisting.xml"));

        TS_ASSERT(objA1.get() == objA2.get());
    }

};


