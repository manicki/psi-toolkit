add_library(formats-bracketing SHARED
    active_elements_printer.cpp
    bracket_printer.cpp
    bracketing_lattice_writer.cpp
)

add_subdirectory(t)

install(TARGETS formats-bracketing DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/bracketing-lattice-writer)
