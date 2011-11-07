add_library(formats-psi SHARED
	psi_lattice_reader.cpp
	psi_lattice_writer.cpp
)

install(TARGETS formats-psi DESTINATION lib/)

