add_library(formats-psi SHARED
    psi_reader/psi_lattice_reader.cpp
    psi_writer/psi_lattice_writer.cpp
)

add_subdirectory(t)

install(TARGETS formats-psi DESTINATION lib/)
install(DIRECTORY psi_reader/data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/psi-lattice-reader)
install(DIRECTORY psi_writer/data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/psi-lattice-writer)
