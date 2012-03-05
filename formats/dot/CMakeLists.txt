add_library(formats-dot SHARED
    dot_lattice_writer.cpp
)

add_subdirectory(t)

install(TARGETS formats-dot DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/dot-writer)

