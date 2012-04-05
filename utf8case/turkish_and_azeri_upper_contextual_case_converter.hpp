#ifndef TURKISH_AND_AZERI_UPPER_CONTEXTUAL_CASE_CONVERTER_HDR
#define TURKISH_AND_AZERI_UPPER_CONTEXTUAL_CASE_CONVERTER_HDR

#include "contextual_case_converter.hpp"

class TurkishAndAzeriUpperContextualCaseConverter: public ContextualCaseConverter {
public:
    virtual ~TurkishAndAzeriUpperContextualCaseConverter();

    virtual const char* convert(
        uint32_t prev_code_point,
        uint32_t code_point,
        uint32_t next_code_point);

private:
    const static uint32_t LATIN_SMALL_LETTER_I;
};


#endif
