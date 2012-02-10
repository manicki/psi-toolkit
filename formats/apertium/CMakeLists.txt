add_library(formats-apertium SHARED
    apertium_reader/apertium_lattice_reader.cpp
    apertium_reader/apertium_deformatter.cpp
    apertium_reader/format_specification.cpp
)

install(TARGETS formats-apertium DESTINATION lib/)
