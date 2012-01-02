get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}")

add_library(tools-recognizers-langguesser-tests
    lang_guesser_tests.cpp
)
target_link_libraries(tools-recognizers-langguesser-tests tools-recognizers-langguesser common-tests)

