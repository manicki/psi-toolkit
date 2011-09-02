add_library(formats-simple 
	simple_lattice_writer.cpp
)
target_link_libraries(formats-simple ${Boost_LIBRARIES} common)
