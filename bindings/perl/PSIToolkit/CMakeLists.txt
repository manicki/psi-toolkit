
include(${SWIG_USE_FILE})

find_package(PerlLibs)
include_directories(${PERL_INCLUDE_PATH})

set(CMAKE_SWIG_FLAGS "")

set(CMAKE_SWIG_CURRENT_LANG_BINDINGS_DIRECTORY "${CMAKE_SWIG_BINDINGS_DIRECTORY}/perl/PSIToolkit")

set(WRAPPER_INTERFACE_FILES
  PSIToolkitSimple.i)

foreach(filename ${WRAPPER_INTERFACE_FILES})
  set_source_files_properties(${filename} PROPERTIES CPLUSPLUS ON)
  set_source_files_properties(${filename} PROPERTIES SWIG_FLAGS "")
endforeach(filename)

set(CXX_FILES PSIToolkitSimplePERL_wrap.cxx)

# turning off warnings in generated files
foreach(filename ${CXX_FILES})
  set_source_files_properties(${filename} PROPERTIES COMPILE_FLAGS "-w")
endforeach(filename)

swig_add_module(Simple perl PSIToolkitSimple.i)

swig_link_libraries(Simple
  common
  formats-utt
  formats-simple
  formats-perl-simple
  formats-psi
  formats-txt
  tools-tokenizers-tp
  tools-segmenters-srx
  tools-parsers-puddle
  framework)

link_directories("${psi-toolkit_BINARY_DIR}/bindings/perl/PSIToolkit")

install(TARGETS Simple DESTINATION share/perl5/site_perl/PSIToolkit)
INSTALL(FILES
  ${CMAKE_SWIG_CURRENT_LANG_BINDINGS_DIRECTORY}/Simple.pm
  DESTINATION share/perl5/site_perl)
