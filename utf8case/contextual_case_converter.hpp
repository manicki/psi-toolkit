#ifndef CONTEXTUAL_CASE_CONVERTER_HDR
#define CONTEXTUAL_CASE_CONVERTER_HDR

#include <boost/cstdint.hpp>

class ContextualCaseConverter {
public:
    virtual ~ContextualCaseConverter() {
    }

    virtual const char* convert(
        uint32_t prev_code_point,
        uint32_t code_point,
        uint32_t next_code_point) = 0;
};

#endif
