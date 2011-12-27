#ifndef SPECIAL_CASING_CONVERTER_HDR
#define SPECIAL_CASING_CONVERTER_HDR

#include "case_tables.hpp"

class SpecialCasingConverter {

public:
    SpecialCasingConverter(size_t tableSize, const SpecialCasingConversionRecord* conversionTable)
        :tableSize_(tableSize), conversionTable_(conversionTable) {
    }

    const char* convert(uint32_t code_point) const;

private:

    size_t tableSize_;
    const SpecialCasingConversionRecord* conversionTable_;
};

#endif
