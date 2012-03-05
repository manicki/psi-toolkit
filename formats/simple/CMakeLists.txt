add_library(formats-simple SHARED
    simple_lattice_writer.cpp
    simple_lattice_writer_stream_output_iterator.cpp
    abstract_simple_data_lattice_writer.cpp
    abstract_simple_data_lattice_writer_output.cpp
    abstract_simple_data_lattice_writer_output_iterator.cpp
)

add_subdirectory(t)

install(TARGETS formats-simple DESTINATION lib/)
