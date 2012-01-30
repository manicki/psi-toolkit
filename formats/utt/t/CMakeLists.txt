get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}")

add_library(formats-utt-tests
    utt_reader_tests.cpp
)

target_link_libraries(formats-utt-tests formats-utt)
