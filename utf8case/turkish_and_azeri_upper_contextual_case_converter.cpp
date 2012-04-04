#include "turkish_and_azeri_upper_contextual_case_converter.hpp"

TurkishAndAzeriUpperContextualCaseConverter::~TurkishAndAzeriUpperContextualCaseConverter() {
}

const char* TurkishAndAzeriUpperContextualCaseConverter::convert(
    uint32_t /*prev_code_point*/,
    uint32_t code_point,
    uint32_t /*next_code_point*/) {

    if (code_point == LATIN_SMALL_LETTER_I)
        return "İ";

    return 0;
}

const uint32_t TurkishAndAzeriUpperContextualCaseConverter::LATIN_SMALL_LETTER_I = 0x0069;
