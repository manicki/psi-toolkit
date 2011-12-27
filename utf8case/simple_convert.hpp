#ifndef SIMPLE_CONVERT_HDR
#define SIMPLE_CONVERT_HDR

#include "general_case_converter.hpp"

std::string simpleConvert(
    const GeneralCaseConverter<std::string::const_iterator, std::back_insert_iterator<std::string> >&
          converter,
    const std::string& s);

std::string simpleHeadConvert(
    const GeneralCaseConverter<std::string::const_iterator, std::back_insert_iterator<std::string> >&
          converter,
    const std::string& s);

std::string simpleTailConvert(
    const GeneralCaseConverter<std::string::const_iterator, std::back_insert_iterator<std::string> >&
          converter,
    const std::string& s);


bool simpleWillBeTouchedWhenConverted(
    const GeneralCaseConverter<std::string::const_iterator, std::back_insert_iterator<std::string> >&
          converter,
    const std::string& s);

bool simpleWillBeTouchedWhenHeadConverted(
    const GeneralCaseConverter<std::string::const_iterator, std::back_insert_iterator<std::string> >&
          converter,
    const std::string& s);

bool simpleWillBeTouchedWhenTailConverted(
    const GeneralCaseConverter<std::string::const_iterator, std::back_insert_iterator<std::string> >&
          converter,
    const std::string& s);




#endif
