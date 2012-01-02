include_directories("..")

add_library(tools-recognizers-langguesser-tests
    lang_guesser_tests.cpp
)
target_link_libraries(tools-recognizers-langguesser-tests tools-recognizers-langguesser common-tests)

