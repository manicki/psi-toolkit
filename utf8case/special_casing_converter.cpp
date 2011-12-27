#include "special_casing_converter.hpp"

const char* SpecialCasingConverter::convert(uint32_t code_point) const {

    for (size_t i = 0; i < tableSize_; ++i) {
        const SpecialCasingConversionRecord* currentRecord = &conversionTable_[i];

        if (code_point < currentRecord->code_point)
            return 0;

        if (code_point == currentRecord->code_point)
            return currentRecord->replacement;
    }

    return 0;
}
