add_library(utf8case SHARED
    case_tables.cpp
    range_based_case_converter.cpp
    simple_convert.cpp
    special_casing_converter.cpp
    string_case_converter_manager.cpp
)

add_subdirectory(t)

