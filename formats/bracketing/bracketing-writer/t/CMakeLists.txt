get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}")
include_directories("${psi-toolkit_SOURCE_DIR}/common/t")

add_library(formats-bracketing-tests
    bracket_printer_tests.cpp
)

target_link_libraries(formats-bracketing-tests formats-bracketing)
