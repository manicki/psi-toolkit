get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}")

add_library(tools-parsers-puddle-tests
        puddle_general_tests.cpp
        puddle_group-alike_actions_tests.cpp
        puddle_non-group_actions_tests.cpp
        puddle_autodelete_tests.cpp
)

target_link_libraries(tools-parsers-puddle-tests
  tools-parsers-puddle
)
