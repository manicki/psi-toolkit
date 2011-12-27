#include "simple_convert.hpp"

std::string simpleConvert(
    const GeneralCaseConverter<std::string::const_iterator, std::back_insert_iterator<std::string> >&
          converter,
    const std::string& s) {

    std::string result;

    converter.convert(s.begin(), s.end(), std::back_inserter(result));

    return result;
}

std::string simpleHeadConvert(
    const GeneralCaseConverter<std::string::const_iterator, std::back_insert_iterator<std::string> >&
          converter,
    const std::string& s) {

    std::string result;

    converter.headConvert(s.begin(), s.end(), std::back_inserter(result));

    return result;
}

std::string simpleTailConvert(
    const GeneralCaseConverter<std::string::const_iterator, std::back_insert_iterator<std::string> >&
          converter,
    const std::string& s) {

    std::string result;

    converter.tailConvert(s.begin(), s.end(), std::back_inserter(result));

    return result;
}


bool simpleWillBeTouchedWhenConverted(
    const GeneralCaseConverter<std::string::const_iterator, std::back_insert_iterator<std::string> >&
          converter,
    const std::string& s) {

    return converter.willBeTouchedWhenConverted(s.begin(), s.end());
}

bool simpleWillBeTouchedWhenHeadConverted(
    const GeneralCaseConverter<std::string::const_iterator, std::back_insert_iterator<std::string> >&
          converter,
    const std::string& s) {

    return converter.willBeTouchedWhenHeadConverted(s.begin(), s.end());
}

bool simpleWillBeTouchedWhenTailConverted(
    const GeneralCaseConverter<std::string::const_iterator, std::back_insert_iterator<std::string> >&
          converter,
    const std::string& s) {

    return converter.willBeTouchedWhenTailConverted(s.begin(), s.end());
}
