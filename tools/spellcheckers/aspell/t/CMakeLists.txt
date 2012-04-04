get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}")

add_library(tools-spellcheckers-aspell-tests
    aspell_tests.cpp
)

target_link_libraries(tools-spellcheckers-aspell-tests tools-spellcheckers-aspell common-tests)

