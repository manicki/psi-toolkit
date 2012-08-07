#include "tests.hpp"

#include "annotation_item.hpp"
#include "av_ai_converter.hpp"
#include "av_matrix.hpp"
#include "lattice.hpp"
#include "registrar.tpl"
#include "zvalue.hpp"


BOOST_AUTO_TEST_SUITE( av_ai_converter )


BOOST_AUTO_TEST_CASE( conversion_int_av_to_ai ) {

    AnnotationItemManager aim;
    Lattice lattice(aim);
    registrar<std::string> reg;
    AV_AI_Converter converter(lattice, reg, reg);
    av_matrix<int, int> avm(0);
    avm.set_attr(1, 2, -1L);
    avm.set_attr(3, 4, -1L);
    AnnotationItem ai = converter.toAnnotationItem(avm);
    BOOST_CHECK_EQUAL(ai.getCategory(), "0");
    BOOST_CHECK_EQUAL(lattice.getAnnotationItemManager().getValueAsString(ai, "1"), "2");
    BOOST_CHECK_EQUAL(lattice.getAnnotationItemManager().getValueAsString(ai, "3"), "4");

    av_matrix<int, int> avm2 = converter.toAVMatrix< av_matrix<int, int> >(ai);
    BOOST_CHECK_EQUAL(avm2.get_cat(), 0);
    BOOST_CHECK_EQUAL(avm2.get_attr(reg.get_id("1")), 2);
    BOOST_CHECK_EQUAL(avm2.get_attr(reg.get_id("3")), 4);

}


BOOST_AUTO_TEST_CASE( conversion_int_ai_to_av ) {

    AnnotationItemManager aim;
    Lattice lattice(aim);
    registrar<std::string> reg;
    AV_AI_Converter converter(lattice, reg, reg);
    AnnotationItem ai("0");
    lattice.getAnnotationItemManager().setValue(ai, "1", INTEGER_TO_ZVALUE(2));
    lattice.getAnnotationItemManager().setValue(ai, "3", INTEGER_TO_ZVALUE(4));
    av_matrix<int, int> avm = converter.toAVMatrix< av_matrix<int, int> >(ai);
    BOOST_CHECK_EQUAL(avm.get_cat(), 0);
    BOOST_CHECK_EQUAL(avm.get_attr(reg.get_id("1")), 2);
    BOOST_CHECK_EQUAL(avm.get_attr(reg.get_id("3")), 4);

    AnnotationItem ai2 = converter.toAnnotationItem(avm);
    BOOST_CHECK_EQUAL(ai2.getCategory(), "0");
    BOOST_CHECK_EQUAL(lattice.getAnnotationItemManager().getValueAsString(ai2, "1"), "2");
    BOOST_CHECK_EQUAL(lattice.getAnnotationItemManager().getValueAsString(ai2, "3"), "4");

}


BOOST_AUTO_TEST_CASE( conversion_partially_string ) {

    AnnotationItemManager aim;
    Lattice lattice(aim);
    registrar<std::string> reg;
    AV_AI_Converter converter(lattice, reg, reg);
    AnnotationItem ai("category");
    lattice.getAnnotationItemManager().setValue(ai, "1", "bbb");
    lattice.getAnnotationItemManager().setValue(ai, "3", "ddd");
    av_matrix<int, zvalue> avm = converter.toAVMatrix< av_matrix<int, zvalue> >(ai);
    AnnotationItem ai2 = converter.toAnnotationItem(avm);
    BOOST_CHECK_EQUAL(ai2.getCategory(), "category");
    BOOST_CHECK_EQUAL(lattice.getAnnotationItemManager().getValueAsString(ai2, "1"), "bbb");
    BOOST_CHECK_EQUAL(lattice.getAnnotationItemManager().getValueAsString(ai2, "3"), "ddd");

}


BOOST_AUTO_TEST_CASE( conversion_string ) {

    AnnotationItemManager aim;
    Lattice lattice(aim);
    registrar<std::string> reg;
    AV_AI_Converter converter(lattice, reg, reg);
    AnnotationItem ai("category");
    lattice.getAnnotationItemManager().setValue(ai, "Aa", "bbb");
    lattice.getAnnotationItemManager().setValue(ai, "Cc", "ddd");
    av_matrix<int, zvalue> avm = converter.toAVMatrix< av_matrix<int, zvalue> >(ai);
    AnnotationItem ai2 = converter.toAnnotationItem(avm);
    BOOST_CHECK_EQUAL(ai2.getCategory(), "category");
    BOOST_CHECK_EQUAL(lattice.getAnnotationItemManager().getValueAsString(ai2, "Aa"), "bbb");
    BOOST_CHECK_EQUAL(lattice.getAnnotationItemManager().getValueAsString(ai2, "Cc"), "ddd");

}


BOOST_AUTO_TEST_SUITE_END()
