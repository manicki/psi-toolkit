set(ALL_SUPPORTED_LANGUAGES perl)
set(CMAKE_SWIG_BINDINGS_DIRECTORY ${CMAKE_BINARY_DIR}/bindings)

foreach(dir ${ALL_SUPPORTED_LANGUAGES})
  add_subdirectory(${dir})
endforeach(dir)
