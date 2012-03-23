add_library(formats-json-simple SHARED
    json_lattice_writer.cpp
    json_lattice_writer_output.cpp
    json_lattice_writer_output_iterator.cpp
)

install(TARGETS formats-json-simple DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/json-simple-lattice-writer)
