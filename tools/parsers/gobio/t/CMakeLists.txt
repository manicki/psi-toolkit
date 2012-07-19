get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}/avmatrix")
include_directories("${PARENT_DIR}/chart")
include_directories("${PARENT_DIR}/parser")
include_directories("${PARENT_DIR}/registrar")
include_directories("${PARENT_DIR}/rules")
include_directories("${PARENT_DIR}/translator")
include_directories("${PARENT_DIR}/trees")
include_directories("${psi-toolkit_SOURCE_DIR}/common/t")
include_directories("${psi-toolkit_SOURCE_DIR}/formats/psi/psi_writer")

add_library(tools-parsers-gobio-tests
    av_ai_converter_tests.cpp
    chart_tests.cpp
    parser_tests.cpp
    tgbg_tests.cpp
)

target_link_libraries(tools-parsers-gobio-tests tools-parsers-gobio)
