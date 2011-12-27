#include "string_case_converter_manager.hpp"

StringCaseConverterManager& StringCaseConverterManager::getInstance() {
    static StringCaseConverterManager instance;

    return instance;
}


StringCaseConverterManager::StringCaseConverterManager() {
}


