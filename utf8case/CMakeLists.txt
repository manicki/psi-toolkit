add_library(utf8case SHARED
    case_tables.cpp
    range_based_case_converter.cpp
    regular_contextual_case_converter.cpp
    simple_convert.cpp
    special_casing_converter.cpp
    string_case_converter_manager.cpp
    turkish_and_azeri_lower_contextual_case_converter.cpp
    turkish_and_azeri_upper_contextual_case_converter.cpp
)

add_subdirectory(t)

install(TARGETS utf8case DESTINATION lib/)
