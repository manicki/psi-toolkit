#include "tests.hpp"

#include "../annotation_item_manager.hpp"

BOOST_AUTO_TEST_CASE( annotation_simple ) {
    AnnotationItemManager annotationItemManager;
    AnnotationItem annotationItem("noun");
    BOOST_CHECK_EQUAL(annotationItemManager.getValue(annotationItem, "case"), "");
    annotationItemManager.setValue(annotationItem, "case", "nominative");
    BOOST_CHECK_EQUAL(annotationItemManager.getValue(annotationItem, "case"), "nominative");
    annotationItemManager.setValue(annotationItem, "case", "genitive");
    BOOST_CHECK_EQUAL(annotationItemManager.getValue(annotationItem, "case"), "genitive");
    annotationItemManager.setValue(annotationItem, "number", "plural");
    BOOST_CHECK_EQUAL(annotationItemManager.getValue(annotationItem, "number"), "plural");

    std::list< std::pair<std::string, std::string> > av
        = annotationItemManager.getValues(annotationItem);
    std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "case");
    BOOST_CHECK_EQUAL((*avi).second, "genitive");
    ++avi;
    BOOST_REQUIRE(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "plural");
};

BOOST_AUTO_TEST_CASE( annotation_compare ) {
    AnnotationItemManager annotationItemManager;
    AnnotationItem ai1("noun");
    AnnotationItem ai2("noun");
    annotationItemManager.setValue(ai1, "case", "nominative");
    annotationItemManager.setValue(ai1, "number", "plural");
    annotationItemManager.setValue(ai2, "number", "plural");
    annotationItemManager.setValue(ai2, "case", "nominative");
    BOOST_CHECK(ai1 == ai2);
};
