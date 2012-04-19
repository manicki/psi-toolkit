add_library(formats-doc-reader-plugin SHARED
    antiword_adapter_impl.cpp
)

install(TARGETS formats-doc-reader-plugin DESTINATION lib/)

