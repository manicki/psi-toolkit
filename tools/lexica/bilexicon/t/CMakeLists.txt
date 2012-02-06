get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}")

add_library(tools-lexica-bilexicon-tests
        test_simple.cpp
)

target_link_libraries(tools-lexica-bilexicon-tests
        tools-lexica-bilexicon
)


