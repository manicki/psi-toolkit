add_library(tools-parsers-link-plugin SHARED
    link_parser_adapter_impl.cpp
)

target_link_libraries(
    tools-parsers-link-plugin
    ${Boost_LIBRARIES}
    ${LINK_GRAMMAR_LIBRARIES}
)

install(TARGETS tools-parsers-link-plugin DESTINATION lib/)

