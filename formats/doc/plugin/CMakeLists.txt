add_library(formats-doc-reader-plugin SHARED
    antiword_adapter_impl.cpp
)

target_link_libraries(
    formats-doc-reader-plugin
)

install(TARGETS formats-doc-reader-plugin DESTINATION lib/)

