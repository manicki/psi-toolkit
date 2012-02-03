get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}/chart")
include_directories("${PARENT_DIR}/parser")
include_directories("${psi-toolkit_SOURCE_DIR}/common/t")

add_library(tools-parsers-gobio-tests
    parser_tests.cpp
)

target_link_libraries(tools-parsers-gobio-tests tools-parsers-gobio)
