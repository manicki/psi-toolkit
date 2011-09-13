add_library(puddle-tests
        puddle_tests.cpp
)

target_link_libraries(puddle-tests
  tools-parsers-puddle
)
