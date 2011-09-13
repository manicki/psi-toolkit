
add_library(morfologik-tests
        morfologik_tests.cpp
        morfologik_tags_parser_tests.cpp
)


target_link_libraries(morfologik-tests
  tools-lemmatizers-morfologik
  common-tests
)

