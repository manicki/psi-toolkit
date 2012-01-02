include_directories("..")

# for including
include_directories("${psi-toolkit_SOURCE_DIR}/common/t")

add_library(tools-lemmatizers-morfologik-tests
    morfologik_tests.cpp
    morfologik_tags_parser_tests.cpp
)

target_link_libraries(tools-lemmatizers-morfologik-tests tools-lemmatizers-morfologik common-tests)

