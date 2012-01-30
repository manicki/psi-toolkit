get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}/psi_reader")
include_directories("${PARENT_DIR}/psi_writer")

add_library(formats-psi-tests
    psi_reader_tests.cpp
)

target_link_libraries(formats-psi-tests formats-psi)
