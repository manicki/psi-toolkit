get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}/chart")
include_directories("${PARENT_DIR}/parser")
include_directories("${psi-toolkit_SOURCE_DIR}/common/t")
include_directories("${psi-toolkit_SOURCE_DIR}/formats/psi/psi_writer")

add_library(tools-parsers-gobio-tests
    chart_tests.cpp
    parser_tests.cpp
)

target_link_libraries(tools-parsers-gobio-tests tools-parsers-gobio)
