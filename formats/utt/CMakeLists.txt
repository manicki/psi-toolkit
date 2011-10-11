add_library(formats-utt SHARED
	utt_lattice_reader.cpp
	utt_quoter.cpp
)

install(TARGETS formats-utt DESTINATION "${psi-toolkit_BINARY_DIR}")
