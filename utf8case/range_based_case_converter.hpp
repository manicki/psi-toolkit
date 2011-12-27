#ifndef RANGE_BASED_CASE_CONVERTED_HDR
#define RANGE_BASED_CASE_CONVERTED_HDR

#include "case_tables.hpp"

class RangeBasedCaseConverter {

public:
    RangeBasedCaseConverter(size_t tableSize, const CaseConversionRecord* conversionTable)
        :tableSize_(tableSize), conversionTable_(conversionTable) {
    }

    uint32_t convert(uint32_t code_point) const;

private:
    const CaseConversionRecord* findRecord_(uint32_t code_point) const;
    uint32_t applyRecord_(const CaseConversionRecord* conversionRecord, uint32_t code_point) const;
    bool shouldBeSkipped_(
        const CaseConversionRecord* conversionRecord, uint32_t code_point) const;
    bool isSkipRecord_(const CaseConversionRecord* conversionRecord) const;
    uint32_t applyDelta_(int32_t delta, uint32_t code_point) const;

    size_t tableSize_;
    const CaseConversionRecord* conversionTable_;
};

#endif
