add_library(formats-txt SHARED
	txt_lattice_reader.cpp
)

install(TARGETS formats-txt DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/txt-reader)
