#include "annotation_item_manager.hpp"


class AnnotationItemTests : public CxxTest::TestSuite {

public:

    void test_simple() {
        AnnotationItemManager annotationItemManager;
        AnnotationItem annotationItem("noun");
        TS_ASSERT_EQUALS(annotationItemManager.getValue(annotationItem, "case"), "");
        annotationItemManager.setValue(annotationItem, "case", "nominative");
        TS_ASSERT_EQUALS(annotationItemManager.getValue(annotationItem, "case"), "nominative");
        annotationItemManager.setValue(annotationItem, "case", "genitive");
        TS_ASSERT_EQUALS(annotationItemManager.getValue(annotationItem, "case"), "genitive");
        annotationItemManager.setValue(annotationItem, "number", "plural");
        TS_ASSERT_EQUALS(annotationItemManager.getValue(annotationItem, "number"), "plural");

        std::list< std::pair<std::string, std::string> >::iterator avi
            = annotationItemManager.getValues(annotationItem).begin();
        TS_ASSERT_EQUALS((*avi).first, "case");
        TS_ASSERT_EQUALS((*avi).second, "genitive");
        ++avi;
        TS_ASSERT_EQUALS((*avi).first, "number");
        TS_ASSERT_EQUALS((*avi).second, "plural");
    }

};
