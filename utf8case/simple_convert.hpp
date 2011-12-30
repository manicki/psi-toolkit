#ifndef SIMPLE_CONVERT_HDR
#define SIMPLE_CONVERT_HDR

#include "general_case_converter.hpp"

typedef GeneralCaseConverter<std::string::const_iterator,
                             std::back_insert_iterator<std::string> > StringGeneralCaseConverter;

std::string simpleConvert(
    const StringGeneralCaseConverter& converter,
    const std::string& s);

std::string simpleHeadConvert(
    const StringGeneralCaseConverter& converter,
    const std::string& s);

std::string simpleTailConvert(
    const StringGeneralCaseConverter& converter,
    const std::string& s);


bool simpleWillBeTouchedWhenConverted(
    const StringGeneralCaseConverter& converter,
    const std::string& s);

bool simpleWillBeTouchedWhenHeadConverted(
    const StringGeneralCaseConverter& converter,
    const std::string& s);

bool simpleWillBeTouchedWhenTailConverted(
    const StringGeneralCaseConverter& converter,
    const std::string& s);

#endif
