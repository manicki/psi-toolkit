add_library(formats-simple SHARED
    simple_lattice_writer.cpp
    simple_lattice_writer_stream_output_iterator.cpp
)

install(TARGETS formats-simple DESTINATION "${psi-toolkit_BINARY_DIR}")
