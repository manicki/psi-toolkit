get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}")

# for including BySpacesCutter
include_directories("${psi-toolkit_SOURCE_DIR}/common/t")

add_library(tools-lemmatizers-sfst-tests
)

target_link_libraries(tools-lemmatizers-sfst-tests tools-lemmatizers-sfst common-tests)

