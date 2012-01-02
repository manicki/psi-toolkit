include_directories("..")

add_library(tools-segmenters-srx-tests
        srx_rules_tests.cpp
)

target_link_libraries(tools-segmenters-srx-tests
        tools-segmenters-srx
)

