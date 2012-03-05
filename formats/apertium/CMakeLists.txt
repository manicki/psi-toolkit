add_library(formats-apertium SHARED
    apertium_reader/apertium_lattice_reader.cpp
    apertium_reader/apertium_deformatter.cpp
    apertium_reader/format_specification.cpp
    apertium_reader/format_rules.cpp
)

install(TARGETS formats-apertium DESTINATION lib/)
install(DIRECTORY apertium_reader/data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/apertium-reader)
