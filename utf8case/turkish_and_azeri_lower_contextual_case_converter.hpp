#ifndef TURKISH_AND_AZERI_LOWER_CONTEXTUAL_CASE_CONVERTER_HDR
#define TURKISH_AND_AZERI_LOWER_CONTEXTUAL_CASE_CONVERTER_HDR

#include "contextual_case_converter.hpp"

class TurkishAndAzeriLowerContextualCaseConverter: public ContextualCaseConverter {
public:
    virtual ~TurkishAndAzeriLowerContextualCaseConverter();

    virtual const char* convert(
        uint32_t prev_code_point,
        uint32_t code_point,
        uint32_t next_code_point);
private:
    const static uint32_t LATIN_CAPITAL_LETTER_I;
    const static uint32_t LATIN_CAPITAL_LETTER_I_WITH_DOT_ABOVE;
    const static uint32_t DOT_ABOVE;
};


#endif
