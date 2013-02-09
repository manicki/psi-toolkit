add_library(tools-lemmatizers-hfst-plugin SHARED
    hfst_adapter_impl.cpp
)

target_link_libraries(
    tools-lemmatizers-hfst-plugin
    hfst
)

install(TARGETS tools-lemmatizers-hfst-plugin DESTINATION lib/)
