get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}")

include_directories("${psi-toolkit_SOURCE_DIR}/common/t")

add_library(tools-lemmatizers-hfst-tests
    hfst_finnish_tags_parser_tests.cpp
    hfst_general_tests.cpp
    hfst_non-finnish_tags_parser_tests.cpp
    hfst_stemmer_tests.cpp
)

target_link_libraries(tools-lemmatizers-hfst-tests tools-lemmatizers-hfst common-tests)
