add_library(formats-pdf SHARED
    pdf_lattice_reader.cpp
    poppler_plugin.cpp
)

target_link_libraries(
    formats-pdf
    common
    common-plugins
)

add_subdirectory(plugin)

install(TARGETS formats-pdf DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/pdf-lattice-reader)
