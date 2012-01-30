add_library(formats-psi SHARED
    psi_reader/psi_lattice_reader.cpp
    psi_writer/psi_lattice_writer.cpp
)

add_subdirectory(t)

install(TARGETS formats-psi DESTINATION lib/)

