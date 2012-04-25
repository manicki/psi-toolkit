include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/link")

add_library(tools-parsers-link SHARED
    link_parser.cpp
    link_parser_plugin.cpp
)

target_link_libraries(
    tools-parsers-link
    common
    common-plugin
)

add_subdirectory(plugin)

install(TARGETS tools-parsers-link DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/link-parser)
