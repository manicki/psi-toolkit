#include "tests.hpp"

#include "simple_convert.hpp"
#include "case_converter_factory.hpp"
#include "string_case_converter_manager.hpp"

BOOST_AUTO_TEST_SUITE( utf8case )

BOOST_AUTO_TEST_CASE( simple_convert_lower ) {

    boost::shared_ptr<StringGeneralCaseConverter> lowerConverter =
        StringCaseConverterManager::getInstance().getLowerCaseConverter("pl");

    BOOST_CHECK_EQUAL(simpleConvert(*lowerConverter, "KOMPUTER"),
                      std::string("komputer"));

    BOOST_CHECK_EQUAL(simpleConvert(*lowerConverter, "ŹDŹBŁO"),
                      std::string("źdźbło"));

    BOOST_CHECK_EQUAL(simpleConvert(*lowerConverter, "Zażółć gęślą JAŹŃ"),
                      std::string("zażółć gęślą jaźń"));

    BOOST_CHECK_EQUAL(simpleConvert(*lowerConverter, "I"),
                      std::string("i"));

    BOOST_CHECK_EQUAL(simpleConvert(*lowerConverter, "БУКВЫ"),
                      std::string("буквы"));


    BOOST_CHECK_EQUAL(simpleHeadConvert(*lowerConverter, "ŹDŹBŁO"),
                      std::string("źDŹBŁO"));

    BOOST_CHECK_EQUAL(simpleTailConvert(*lowerConverter, "ŹDŹBŁO"),
                      std::string("Źdźbło"));


    BOOST_CHECK_EQUAL(simpleConvert(*lowerConverter, "Ś"),
                      std::string("ś"));

    BOOST_CHECK_EQUAL(simpleHeadConvert(*lowerConverter, "Ś"),
                      std::string("ś"));

    BOOST_CHECK_EQUAL(simpleTailConvert(*lowerConverter, "Ś"),
                      std::string("Ś"));


    BOOST_CHECK_EQUAL(simpleConvert(*lowerConverter, ""),
                      std::string(""));

    BOOST_CHECK_EQUAL(simpleHeadConvert(*lowerConverter, ""),
                      std::string(""));

    BOOST_CHECK_EQUAL(simpleTailConvert(*lowerConverter, ""),
                      std::string(""));

}


BOOST_AUTO_TEST_CASE( will_be_touched ) {
    boost::shared_ptr<StringGeneralCaseConverter> upperConverter =
        StringCaseConverterManager::getInstance().getUpperCaseConverter("pl");

    BOOST_CHECK(simpleWillBeTouchedWhenConverted(*upperConverter, "KOMPUTEr"));
    BOOST_CHECK(simpleWillBeTouchedWhenTailConverted(*upperConverter, "KOMPUTEr"));
    BOOST_CHECK(!simpleWillBeTouchedWhenHeadConverted(*upperConverter, "KOMPUTEr"));

    BOOST_CHECK(!simpleWillBeTouchedWhenConverted(*upperConverter, "KOMPUTER"));
    BOOST_CHECK(!simpleWillBeTouchedWhenTailConverted(*upperConverter, "KOMPUTER"));
    BOOST_CHECK(!simpleWillBeTouchedWhenHeadConverted(*upperConverter, "KOMPUTER"));

    BOOST_CHECK(simpleWillBeTouchedWhenConverted(*upperConverter, "śNIEG"));
    BOOST_CHECK(!simpleWillBeTouchedWhenTailConverted(*upperConverter, "śNIEG"));
    BOOST_CHECK(simpleWillBeTouchedWhenHeadConverted(*upperConverter, "śNIEG"));

    BOOST_CHECK(simpleWillBeTouchedWhenConverted(*upperConverter, "ź"));
    BOOST_CHECK(!simpleWillBeTouchedWhenTailConverted(*upperConverter, "ź"));
    BOOST_CHECK(simpleWillBeTouchedWhenHeadConverted(*upperConverter, "ź"));

    BOOST_CHECK(!simpleWillBeTouchedWhenConverted(*upperConverter, ""));
    BOOST_CHECK(!simpleWillBeTouchedWhenTailConverted(*upperConverter, ""));
    BOOST_CHECK(!simpleWillBeTouchedWhenHeadConverted(*upperConverter, ""));
}

BOOST_AUTO_TEST_CASE( simple_convert_upper ) {

    boost::shared_ptr<StringGeneralCaseConverter> upperConverter =
        StringCaseConverterManager::getInstance().getUpperCaseConverter("pl");

    BOOST_CHECK_EQUAL(simpleConvert(*upperConverter, "komputer"),
                      std::string("KOMPUTER"));

    BOOST_CHECK_EQUAL(simpleConvert(*upperConverter, "źdźbło"),
                      std::string("ŹDŹBŁO"));

    BOOST_CHECK_EQUAL(simpleConvert(*upperConverter, "daß"),
                      std::string("DASS"));

    BOOST_CHECK_EQUAL(simpleConvert(*upperConverter, "ﬃ"),
                      std::string("FFI"));

}


BOOST_AUTO_TEST_CASE( simple_convert_title ) {

    boost::shared_ptr<StringGeneralCaseConverter> titleConverter =
        StringCaseConverterManager::getInstance().getTitleCaseConverter("pl");

    BOOST_CHECK_EQUAL(simpleConvert(*titleConverter, "źdźbło"),
                      std::string("ŹDŹBŁO"));

    BOOST_CHECK_EQUAL(simpleConvert(*titleConverter, "daß"),
                      std::string("DASs"));

    BOOST_CHECK_EQUAL(simpleConvert(*titleConverter, "ﬃ"),
                      std::string("Ffi"));

}


BOOST_AUTO_TEST_SUITE_END()
