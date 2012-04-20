add_library(formats-gv SHARED
    gv_lattice_writer.cpp
    graphviz_plugin.cpp
)

target_link_libraries(
    formats-gv
    common
    common-plugins
)

add_subdirectory(t)
add_subdirectory(plugin)

install(TARGETS formats-gv DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/gv-writer)
