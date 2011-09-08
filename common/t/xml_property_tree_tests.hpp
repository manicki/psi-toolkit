#include "xml_property_tree.hpp"

#include <boost/scoped_ptr.hpp>

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

//        boost::scoped_ptr<XmlPropertyTree> xmlTree;


//        TS_ASSERT_THROWS_ANYTHING(
//            xmlTree.reset(new XmlPropertyTree(ROOT_DIR "common/t/broken.xml")));
    }

    void testNonexisting() {

        boost::scoped_ptr<XmlPropertyTree> xmlTree;

        TS_ASSERT_THROWS_ANYTHING(
            xmlTree.reset(new XmlPropertyTree(ROOT_DIR "common/t/nonexisting.xml")));
    }

    void testSpaces() {
        XmlPropertyTree xmlTree(ROOT_DIR "common/t/spaces.xml");

        TS_ASSERT_EQUALS(
            xmlTree.get<std::string>("root.foo"), "  ");
    }
};
