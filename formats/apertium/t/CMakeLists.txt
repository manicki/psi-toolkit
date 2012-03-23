get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}/apertium_reader")
include_directories("${psi-toolkit_SOURCE_DIR}/common/t")

add_library(formats-apertium-tests
    format_specification_tests.cpp
)

target_link_libraries(formats-apertium-tests formats-apertium)
