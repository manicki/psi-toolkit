#include "simple_convert.hpp"

std::string simpleConvert(
    const StringGeneralCaseConverter& converter,
    const std::string& s) {

    std::string result;

    converter.convert(s.begin(), s.end(), std::back_inserter(result));

    return result;
}

std::string simpleHeadConvert(
    const StringGeneralCaseConverter& converter,
    const std::string& s) {

    std::string result;

    converter.headConvert(s.begin(), s.end(), std::back_inserter(result));

    return result;
}

std::string simpleTailConvert(
    const StringGeneralCaseConverter& converter,
    const std::string& s) {

    std::string result;

    converter.tailConvert(s.begin(), s.end(), std::back_inserter(result));

    return result;
}


bool simpleWillBeTouchedWhenConverted(
    const StringGeneralCaseConverter& converter,
    const std::string& s) {

    return converter.willBeTouchedWhenConverted(s.begin(), s.end());
}

bool simpleWillBeTouchedWhenHeadConverted(
    const StringGeneralCaseConverter& converter,
    const std::string& s) {

    return converter.willBeTouchedWhenHeadConverted(s.begin(), s.end());
}

bool simpleWillBeTouchedWhenTailConverted(
    const StringGeneralCaseConverter& converter,
    const std::string& s) {

    return converter.willBeTouchedWhenTailConverted(s.begin(), s.end());
}
