add_library(formats-psi SHARED
	psi_reader/psi_lattice_reader.cpp
	psi_writer/psi_lattice_writer.cpp
)

install(TARGETS formats-psi DESTINATION lib/)

