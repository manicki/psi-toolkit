add_library(formats-gv-writer-plugin SHARED
    djvu_adapter_impl.cpp
)

target_link_libraries(
    formats-gv-writer-plugin
    ${Boost_LIBRARIES}
    ${GRAPHVIZ_LIBRARIES}
)

install(TARGETS formats-gv-writer-plugin DESTINATION lib/)

