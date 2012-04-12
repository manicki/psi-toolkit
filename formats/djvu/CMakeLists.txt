add_library(formats-djvu SHARED
    djvu_lattice_reader.cpp
)

target_link_libraries(formats-djvu ${Boost_LIBRARIES} ${DJVULIBRE_LIBRARIES} common)

install(TARGETS formats-djvu DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/djvu-lattice-reader)
