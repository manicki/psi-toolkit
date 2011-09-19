add_library(formats-simple
    simple_lattice_writer.cpp
    simple_lattice_writer_stream_output_iterator.cpp
)
target_link_libraries(formats-simple ${Boost_LIBRARIES} common)
