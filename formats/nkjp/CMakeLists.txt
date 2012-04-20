add_library(formats-nkjp SHARED
    nkjp_lattice_reader.cpp
)

install(TARGETS formats-nkjp DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/nkjp-lattice-reader)
