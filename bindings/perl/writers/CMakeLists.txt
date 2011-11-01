add_library(formats-perl-simple SHARED
    perl_simple_lattice_writer.cpp
    perl_simple_lattice_writer_output_iterator.cpp
    perl_lattice_writer_output.cpp
)

target_link_libraries(formats-perl-simple ${PERL_LIBRARY})

find_package(SWIG)
include(${SWIG_USE_FILE})

find_package(PerlLibs)
include_directories(${PERL_INCLUDE_PATH})

install(TARGETS formats-perl-simple DESTINATION lib/)
