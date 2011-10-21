find_package(SWIG)
set(HAVE_PERL_BINDINGS 1)

if (HAVE_PERL_BINDINGS)
  set(WRAPPERS_CPP_DIRS writers)
  foreach(dir ${WRAPPERS_CPP_DIRS})
    include_directories("${psi-toolkit_SOURCE_DIR}/bindings/perl/${dir}")
    link_directories("${psi-toolkit_BINARY_DIR}/bindings/perl/${dir}")
    add_subdirectory(${dir})
  endforeach(dir)

  include(${SWIG_USE_FILE})
  
  find_package(PerlLibs)
  include_directories(${PERL_INCLUDE_PATH})

  set(CMAKE_SWIG_FLAGS "")

  set(CMAKE_SWIG_CURRENT_LANG_BINDINGS_DIRECTORY "${CMAKE_SWIG_BINDINGS_DIRECTORY}/perl")

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


  install(TARGETS Simple DESTINATION "${psi-toolkit_BINARY_DIR}")
  INSTALL(FILES
    ${CMAKE_SWIG_CURRENT_LANG_BINDINGS_DIRECTORY}/Simple.pm
    DESTINATION "${psi-toolkit_BINARY_DIR}/PSIToolkit")
endif(HAVE_PERL_BINDINGS)
