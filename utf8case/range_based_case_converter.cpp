#include "range_based_case_converter.hpp"

uint32_t RangeBasedCaseConverter::convert(uint32_t code_point) const {

    const CaseConversionRecord* conversionRecord = findRecord_(code_point);

    return
        conversionRecord == 0
        ? code_point
        : applyRecord_(conversionRecord, code_point);
}

const CaseConversionRecord* RangeBasedCaseConverter::findRecord_(uint32_t code_point) const {

    for (size_t i = 0; i < tableSize_; ++i) {
        const CaseConversionRecord* currentRecord = &conversionTable_[i];

        if (code_point < currentRecord->lo_code_point)
            return 0;

        if (code_point <= currentRecord->hi_code_point)
            return currentRecord;
    }

    return 0;
}

uint32_t RangeBasedCaseConverter::applyRecord_(
    const CaseConversionRecord* conversionRecord, uint32_t code_point) const {

    if (shouldBeSkipped_(conversionRecord, code_point))
        return code_point;

    return applyDelta_(conversionRecord->delta, code_point);
}


bool RangeBasedCaseConverter::shouldBeSkipped_(
    const CaseConversionRecord* conversionRecord, uint32_t code_point) const {

    return
        isSkipRecord_(conversionRecord)
        && code_point % 2 != conversionRecord->lo_code_point % 2;
}

bool RangeBasedCaseConverter::isSkipRecord_(const CaseConversionRecord* conversionRecord) const {
    return
        conversionRecord->delta == EVEN_ODD_SKIP
        || conversionRecord->delta == ODD_EVEN_SKIP;
}

uint32_t RangeBasedCaseConverter::applyDelta_(int32_t delta, uint32_t code_point) const {
    switch (delta) {
    case EVEN_ODD:
    case EVEN_ODD_SKIP:
        return code_point % 2 == 0 ? code_point+1 : code_point-1;
    case ODD_EVEN:
    case ODD_EVEN_SKIP:
        return code_point % 2 == 1 ? code_point+1 : code_point-1;
    default:
        return code_point + delta;
    }
}
