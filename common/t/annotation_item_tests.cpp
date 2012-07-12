#include "tests.hpp"

#include "annotation_item_manager.hpp"

BOOST_AUTO_TEST_CASE( annotation_simple ) {
    AnnotationItemManager manager;
    AnnotationItem annotationItem("noun");
    BOOST_CHECK_EQUAL(manager.getValue(annotationItem, "case"), NULL_ZVALUE);
    BOOST_CHECK_EQUAL(manager.getValueAsString(annotationItem, "case"), "NULL_ZVALUE");
    manager.setValue(annotationItem, "case", "nominative");
    BOOST_CHECK_EQUAL(manager.getValueAsString(annotationItem, "case"), "nominative");
    manager.setValue(annotationItem, "case", "genitive");
    BOOST_CHECK_EQUAL(manager.getValueAsString(annotationItem, "case"), "genitive");
    manager.setValue(annotationItem, "number", "plural");
    BOOST_CHECK_EQUAL(manager.getValueAsString(annotationItem, "number"), "plural");
    manager.setValue(annotationItem, "count", INTEGER_TO_ZVALUE(123));
    BOOST_CHECK_EQUAL(ZVALUE_TO_INTEGER(manager.getValue(annotationItem, "count")), 123);
    std::list< std::pair<std::string, std::string> > av = manager.getValues(annotationItem);
    std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
    BOOST_CHECK_EQUAL((*avi).first, "case");
    BOOST_CHECK_EQUAL((*avi).second, "genitive");
    ++avi;
    BOOST_REQUIRE(avi != av.end());
    BOOST_CHECK_EQUAL((*avi).first, "number");
    BOOST_CHECK_EQUAL((*avi).second, "plural");
};

BOOST_AUTO_TEST_CASE( annotation_compare ) {
    AnnotationItemManager manager;
    AnnotationItem ai1("noun");
    AnnotationItem ai2("noun");
    manager.setValue(ai1, "case", "nominative");
    manager.setValue(ai1, "number", "plural");
    manager.setValue(ai2, "number", "plural");
    manager.setValue(ai2, "case", "nominative");
    BOOST_CHECK(ai1 == ai2);
};

BOOST_AUTO_TEST_CASE( annotation_text ) {
    AnnotationItemManager manager;
    AnnotationItem ai("noun", StringFrag("ala"));
    manager.setValue(ai, "case", "nominative");
    manager.setValue(ai, "number", "plural");
    BOOST_CHECK_EQUAL(ai.getCategory(), "noun");
    BOOST_CHECK_EQUAL(ai.getText(), "ala");
};

BOOST_AUTO_TEST_CASE( annotation_text_as_string_frag ) {
    AnnotationItemManager manager;
    std::string text("Ala ma kota.");
    AnnotationItem ai("noun", StringFrag(text, 4, 2));
    manager.setValue(ai, "case", "nominative");
    manager.setValue(ai, "number", "plural");
    BOOST_CHECK_EQUAL(ai.getCategory(), "noun");
    BOOST_CHECK_EQUAL(ai.getText(), "ma");
    BOOST_CHECK_EQUAL(ai.getTextAsStringFrag().str(), "ma");
};

BOOST_AUTO_TEST_CASE( zvalue_conversion ) {
    AnnotationItemManager manager;
    std::string text("Ala ma kota.");
    zvalue ztext = manager.stringToZvalue(text);
    std::string text2 = manager.zvalueToString(ztext);
    BOOST_CHECK_EQUAL(text2, text);
    zvalue ztext2 = manager.stringToZvalue(text2);
    BOOST_CHECK_EQUAL(ztext2, ztext);
};
