#ifndef STRING_CASE_CONVERTER_MANAGER_HDR
#define STRING_CASE_CONVERTER_MANAGER_HDR

#include "case_converter_factory.hpp"

class StringCaseConverterManager : public CaseConverterFactory<
    std::string::const_iterator, std::back_insert_iterator<std::string> > {

public:
    static StringCaseConverterManager& getInstance();

private:
    StringCaseConverterManager();
};

#endif
