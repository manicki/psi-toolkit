add_library(formats-gv SHARED
    gv_lattice_writer.cpp
)

target_link_libraries(formats-gv ${Boost_LIBRARIES} ${GRAPHVIZ_LIBRARIES} common)

add_subdirectory(t)

install(TARGETS formats-gv DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/gv-writer)
