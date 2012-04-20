add_library(formats-djvu SHARED
    djvu_lattice_reader.cpp
    djvu_plugin.cpp
)

target_link_libraries(
    formats-djvu
    common
    common-plugins
)

add_subdirectory(plugin)

install(TARGETS formats-djvu DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/djvu-lattice-reader)
