get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}")

add_library(tools-taggers-metagger-tests
    me_tagger_tests.cpp
)

target_link_libraries(tools-taggers-metagger-tests
    tools-taggers-metagger
)
