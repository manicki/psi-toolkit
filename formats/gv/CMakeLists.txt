add_library(formats-gv SHARED
    gv_lattice_writer.cpp
)

add_subdirectory(t)

install(TARGETS formats-gv DESTINATION lib/)
