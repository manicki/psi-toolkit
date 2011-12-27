#include "tests.hpp"

#include "../special_casing_converter.hpp"

BOOST_AUTO_TEST_SUITE( utf8case )

BOOST_AUTO_TEST_CASE( special_casing_converter ) {
    SpecialCasingConverter converter(UPPER_SPECIAL_CASING_SIZE,
                                     UPPER_SPECIAL_CASING);

    const uint32_t COMMA_CODE_POINT = 44U;
    BOOST_CHECK_EQUAL(converter.convert(COMMA_CODE_POINT), (const char*)0);

    const uint32_t UPPER_F_CODE_POINT = 70U;
    const uint32_t LOWER_F_CODE_POINT = 102U;
    BOOST_CHECK_EQUAL(converter.convert(UPPER_F_CODE_POINT), (const char*)0);
    BOOST_CHECK_EQUAL(converter.convert(LOWER_F_CODE_POINT), (const char*)0);

    const uint32_t UPPER_SHCHA_CODE_POINT = 1065U;
    const uint32_t LOWER_SHCHA_CODE_POINT = 1097U;
    BOOST_CHECK_EQUAL(converter.convert(UPPER_SHCHA_CODE_POINT), (const char*)0);
    BOOST_CHECK_EQUAL(converter.convert(LOWER_SHCHA_CODE_POINT), (const char*)0);

    const uint32_t ESZET_CODE_POINT = 223U;
    BOOST_CHECK_EQUAL(converter.convert(ESZET_CODE_POINT), "SS");
}

BOOST_AUTO_TEST_SUITE_END()
