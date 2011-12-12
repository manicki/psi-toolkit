if (HAVE_SWIG_BINDINGS)
  set(ALL_SUPPORTED_LANGUAGES perl python)
  set(CMAKE_SWIG_BINDINGS_DIRECTORY ${CMAKE_BINARY_DIR}/bindings)

  foreach(dir ${ALL_SUPPORTED_LANGUAGES})
    add_subdirectory(${dir})
  endforeach(dir)
endif(HAVE_SWIG_BINDINGS)
