add_library(formats-pdf SHARED
    pdf_lattice_reader.cpp
)

install(TARGETS formats-pdf DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/pdf-lattice-reader)
