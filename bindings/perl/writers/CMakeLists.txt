include_directories("${psi-toolkit_SOURCE_DIR}/formats/simple")

add_library(formats-perl-simple SHARED
    perl_simple_lattice_writer.cpp
    perl_simple_lattice_writer_output_iterator.cpp
    perl_lattice_writer_output.cpp
)

target_link_libraries(formats-perl-simple
  formats-simple
  ${PERL_LIBRARY})

find_package(SWIG)
include(${SWIG_USE_FILE})

find_package(PerlLibs)

install(TARGETS formats-perl-simple DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/perl-simple-lattice-writer)
