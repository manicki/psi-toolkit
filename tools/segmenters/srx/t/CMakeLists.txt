
get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}")

add_library(tools-segmenters-srx-tests
        srx_rules_tests.cpp
)

target_link_libraries(tools-segmenters-srx-tests
        tools-segmenters-srx
)

