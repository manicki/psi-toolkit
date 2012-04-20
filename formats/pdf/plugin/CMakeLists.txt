add_library(formats-pdf-reader-plugin SHARED
    poppler_adapter_impl.cpp
)

target_link_libraries(
    formats-pdf-reader-plugin
    ${Boost_LIBRARIES}
    ${POPPLER_LIBRARIES}
)

install(TARGETS formats-pdf-reader-plugin DESTINATION lib/)

