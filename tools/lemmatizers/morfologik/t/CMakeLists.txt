get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}")

# for including BySpacesCutter
include_directories("${psi-toolkit_SOURCE_DIR}/common/t")

add_library(tools-lemmatizers-morfologik-tests
    morfologik_tests.cpp
    morfologik_tags_parser_tests.cpp
)

target_link_libraries(tools-lemmatizers-morfologik-tests tools-lemmatizers-morfologik common-tests)

