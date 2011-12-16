add_library(tools-lemmatizers-morfologik-tests
    morfologik_tests.cpp
    morfologik_tags_parser_tests.cpp
)

target_link_libraries(tools-lemmatizers-morfologik-tests tools-lemmatizers-morfologik common-tests)

