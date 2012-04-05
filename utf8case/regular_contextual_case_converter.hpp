#ifndef REGULAR_CONTEXTUAL_CASE_CONVERTER_HDR
#define REGULAR_CONTEXTUAL_CASE_CONVERTER_HDR

#include "contextual_case_converter.hpp"

class RegularContextualCaseConverter: public ContextualCaseConverter {
public:
    virtual ~RegularContextualCaseConverter();

    virtual const char* convert(
        uint32_t prev_code_point,
        uint32_t code_point,
        uint32_t next_code_point);
};

#endif
