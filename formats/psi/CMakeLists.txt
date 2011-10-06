add_library(formats-psi
    psi_lattice_reader.cpp
    psi_lattice_writer.cpp
)
target_link_libraries(formats-psi ${Boost_LIBRARIES} common)
