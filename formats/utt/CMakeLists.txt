add_library(formats-utt SHARED
    utt_lattice_reader.cpp
    utt_quoter.cpp
)

add_subdirectory(t)

install(TARGETS formats-utt DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/utt-lattice-reader)
