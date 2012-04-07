#include "turkish_and_azeri_lower_contextual_case_converter.hpp"

TurkishAndAzeriLowerContextualCaseConverter::~TurkishAndAzeriLowerContextualCaseConverter() {
}

const char* TurkishAndAzeriLowerContextualCaseConverter::convert(
    uint32_t prev_code_point,
    uint32_t code_point,
    uint32_t next_code_point) {

    if (code_point == LATIN_CAPITAL_LETTER_I && next_code_point != DOT_ABOVE)
        return "ı";

    if (code_point == LATIN_CAPITAL_LETTER_I_WITH_DOT_ABOVE)
        return "i";

    if (code_point == DOT_ABOVE && prev_code_point == LATIN_CAPITAL_LETTER_I)
        return "";

    return 0;
}

const uint32_t TurkishAndAzeriLowerContextualCaseConverter::LATIN_CAPITAL_LETTER_I = 0x0049;


const uint32_t TurkishAndAzeriLowerContextualCaseConverter::LATIN_CAPITAL_LETTER_I_WITH_DOT_ABOVE =
    0x0130;

const uint32_t TurkishAndAzeriLowerContextualCaseConverter::DOT_ABOVE = 0x0307;
