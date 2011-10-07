add_library(formats-txt SHARED
	txt_lattice_reader.cpp
)

install(TARGETS formats-txt DESTINATION "${psi-toolkit_BINARY_DIR}")
