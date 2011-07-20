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
    }

};
