add_library(tools-parsers-puddle-tests
        puddle_general_tests.cpp
        puddle_actions_tests.cpp
)

target_link_libraries(tools-parsers-puddle-tests
  tools-parsers-puddle
)
