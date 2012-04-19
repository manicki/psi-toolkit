add_library(formats-djvu-reader-plugin SHARED
    djvu_adapter_impl.cpp
)

target_link_libraries(
    formats-djvu-reader-plugin
    ${Boost_LIBRARIES}
    ${DJVULIBRE_LIBRARIES}
)

install(TARGETS formats-djvu-reader-plugin DESTINATION lib/)

