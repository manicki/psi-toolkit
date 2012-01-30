get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}")
include_directories("${psi-toolkit_SOURCE_DIR}/common/t")

add_library(formats-simple-tests
    lattice_iter_writer_tests.cpp
    simple_writer_tests.cpp
)

target_link_libraries(formats-simple-tests formats-simple)
