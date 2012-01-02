#include "tests.hpp"

#include "range_based_case_converter.hpp"

BOOST_AUTO_TEST_SUITE( utf8case )

void lower_single_letter_checker(uint32_t lower_code_point, uint32_t upper_code_point) {
    RangeBasedCaseConverter converter(LOWER_CASE_RANGES_SIZE,
                                      LOWER_CASE_RANGES);

    BOOST_CHECK_EQUAL(converter.convert(upper_code_point), lower_code_point);
    BOOST_CHECK_EQUAL(converter.convert(lower_code_point), lower_code_point);
}

BOOST_AUTO_TEST_CASE( range_based_case_converter ) {
    RangeBasedCaseConverter converter(LOWER_CASE_RANGES_SIZE,
                                      LOWER_CASE_RANGES);

    const uint32_t COMMA_CODE_POINT = 44U;
    BOOST_CHECK_EQUAL(converter.convert(COMMA_CODE_POINT), COMMA_CODE_POINT);

    const uint32_t UPPER_F_CODE_POINT = 70U;
    const uint32_t LOWER_F_CODE_POINT = 102U;
    lower_single_letter_checker(LOWER_F_CODE_POINT, UPPER_F_CODE_POINT);

    const uint32_t UPPER_A_CODE_POINT = 65U;
    const uint32_t LOWER_A_CODE_POINT = 97U;
    lower_single_letter_checker(LOWER_A_CODE_POINT, UPPER_A_CODE_POINT);

    const uint32_t UPPER_Z_CODE_POINT = 90U;
    const uint32_t LOWER_Z_CODE_POINT = 122U;
    lower_single_letter_checker(LOWER_Z_CODE_POINT, UPPER_Z_CODE_POINT);

    const uint32_t UPPER_E_OGONEK_CODE_POINT = 280U;
    const uint32_t LOWER_E_OGONEK_CODE_POINT = 281U;
    lower_single_letter_checker(LOWER_E_OGONEK_CODE_POINT, UPPER_E_OGONEK_CODE_POINT);

    const uint32_t UPPER_SHCHA_CODE_POINT = 1065U;
    const uint32_t LOWER_SHCHA_CODE_POINT = 1097U;
    lower_single_letter_checker(LOWER_SHCHA_CODE_POINT, UPPER_SHCHA_CODE_POINT);
}

BOOST_AUTO_TEST_SUITE_END()
