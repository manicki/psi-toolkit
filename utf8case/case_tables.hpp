#ifndef CASE_TABLES_HDR
#define CASE_TABLES_HDR

#include <boost/cstdint.hpp>

enum {
  EVEN_ODD = 1,
  ODD_EVEN = -1,
  EVEN_ODD_SKIP = 1<<30,
  ODD_EVEN_SKIP,
};

struct CaseConversionRecord {
    uint32_t lo_code_point;
    uint32_t hi_code_point;
    int32_t delta;
};

struct SpecialCasingConversionRecord {
    uint32_t code_point;
    const char* replacement;
};

extern const size_t LOWER_CASE_RANGES_SIZE;
extern const CaseConversionRecord LOWER_CASE_RANGES[];

extern const size_t UPPER_CASE_RANGES_SIZE;
extern const CaseConversionRecord UPPER_CASE_RANGES[];

extern const size_t TITLE_CASE_RANGES_SIZE;
extern const CaseConversionRecord TITLE_CASE_RANGES[];

extern const size_t LOWER_SPECIAL_CASING_SIZE;
extern const SpecialCasingConversionRecord LOWER_SPECIAL_CASING[];

extern const size_t TITLE_SPECIAL_CASING_SIZE;
extern const SpecialCasingConversionRecord TITLE_SPECIAL_CASING[];

extern const size_t UPPER_SPECIAL_CASING_SIZE;
extern const SpecialCasingConversionRecord UPPER_SPECIAL_CASING[];

#endif
