#include "regular_contextual_case_converter.hpp"

RegularContextualCaseConverter::~RegularContextualCaseConverter() {
}

const char* RegularContextualCaseConverter::convert(
    uint32_t /*prev_code_point*/,
    uint32_t /*code_point*/,
    uint32_t /*next_code_point*/) {
    return 0;
}
