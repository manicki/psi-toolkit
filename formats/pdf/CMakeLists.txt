add_library(formats-pdf SHARED
    pdf_lattice_reader.cpp
)

target_link_libraries(formats-pdf ${Boost_LIBRARIES} ${POPPLER_LIBRARIES} common)

install(TARGETS formats-pdf DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/pdf-lattice-reader)
