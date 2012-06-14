cmake_minimum_required(VERSION 2.8)

find_package(SWIG)

if (SWIG_FOUND)
  include(${SWIG_USE_FILE})

  find_package(PythonLibs)
  find_package(PythonInterp)

  # getting installation path
  execute_process(
    COMMAND
    ${PYTHON_EXECUTABLE} -c "import sys; print('%s.%s' % sys.version_info[0:2])"
    OUTPUT_VARIABLE PYTHON_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

  set(INSTALL_PYTHON_MODULES_PATH "lib/python${PYTHON_VERSION}/dist-packages")
  if ( "${CMAKE_INSTALL_RPATH}" MATCHES "/local(/|$)")
    STRING(REGEX REPLACE "((^|/)local/)" "" INSTALL_PYTHON_MODULES_PATH ${INSTALL_PYTHON_MODULES_PATH})
  endif()

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

  swig_add_module(PSIToolkit python PSIToolkitSimple.i)

  swig_link_libraries(PSIToolkit
    common
    formats-utt
    formats-simple
    formats-psi
    formats-txt
    tools-tokenizers-tp
    tools-segmenters-srx
    tools-parsers-puddle
    framework)

  INSTALL(FILES
    ${CMAKE_SWIG_CURRENT_LANG_BINDINGS_DIRECTORY}/_PSIToolkit.so
    DESTINATION ${INSTALL_PYTHON_MODULES_PATH})
  INSTALL(FILES
    ${CMAKE_SWIG_CURRENT_LANG_BINDINGS_DIRECTORY}/PSIToolkit.py
    DESTINATION ${INSTALL_PYTHON_MODULES_PATH})


endif(SWIG_FOUND)