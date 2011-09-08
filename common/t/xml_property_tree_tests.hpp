#include "xml_property_tree.hpp"

#include <boost/scoped_ptr.hpp>
#include <boost/version.hpp>

class XmlPropertyTreeTests : public CxxTest::TestSuite {

public:

    void testSimple() {
        XmlPropertyTree xmlTree(ROOT_DIR "common/t/simple.xml");

        TS_ASSERT_EQUALS(
            xmlTree.get<std::string>("root.foo.bar"),
            "Źdźbło & źdźbło");
    }

    void testBroken() {
        XmlPropertyTree xmlTree(ROOT_DIR "common/t/broken.xml");

        TS_ASSERT_EQUALS(
            xmlTree.get<std::string>("root.foo"),
            "Broken XML File.");
    }

    void testNonexisting() {

        boost::scoped_ptr<XmlPropertyTree> xmlTree;

        TS_ASSERT_THROWS_ANYTHING(
            xmlTree.reset(new XmlPropertyTree(ROOT_DIR "common/t/nonexisting.xml")));
    }


    void testSpaces() {
        XmlPropertyTree xmlTree(ROOT_DIR "common/t/spaces.xml");

        // in Boost 1.42 all spaces are trimmed (a bug??)
#if BOOST_VERSION != 14200
        TS_ASSERT_EQUALS(
            xmlTree.get<std::string>("root.foo"), "  ");
#endif
    }


    void testSpaceEntity() {
        XmlPropertyTree xmlTree(ROOT_DIR "common/t/space-entity.xml");

        TS_ASSERT_EQUALS(
            xmlTree.get<std::string>("root.foo"), "   ");
    }

};
