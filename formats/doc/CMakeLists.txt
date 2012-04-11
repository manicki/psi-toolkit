add_library(formats-doc SHARED
    doc_lattice_reader.cpp
)

target_link_libraries(formats-doc ${Boost_LIBRARIES} common)

install(TARGETS formats-doc DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/doc-lattice-reader)
