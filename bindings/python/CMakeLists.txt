cmake_minimum_required(VERSION 2.8)

find_package(SWIG)

if (SWIG_FOUND)
  include(${SWIG_USE_FILE})

  find_package(PythonLibs)
  include_directories(${PYTHON_INCLUDE_DIRS})

  set(CMAKE_SWIG_FLAGS "")

  set(CMAKE_SWIG_CURRENT_LANG_BINDINGS_DIRECTORY "${CMAKE_SWIG_BINDINGS_DIRECTORY}/python")

  set(WRAPPER_INTERFACE_FILES
    PSIToolkitSimple.i)

  foreach(filename ${WRAPPER_INTERFACE_FILES})
    set_source_files_properties(${filename} PROPERTIES CPLUSPLUS ON)
    set_source_files_properties(${filename} PROPERTIES SWIG_FLAGS "")
  endforeach(filename)

  set(CXX_FILES PSIToolkitSimplePYTHON_wrap.cxx)

  # turning off warnings in generated files
  foreach(filename ${CXX_FILES})
    set_source_files_properties(${filename} PROPERTIES COMPILE_FLAGS "-w")
  endforeach(filename)

  swig_add_module(Simple python PSIToolkitSimple.i)

  swig_link_libraries(Simple
    common
    formats-utt
    formats-simple
    formats-psi
    formats-txt
    tools-tokenizers-tp
    tools-segmenters-srx
    tools-parsers-puddle
    framework)



endif(SWIG_FOUND)
