if (HAVE_SWIG_BINDINGS)
  set (CMAKE_SWIG_BINDINGS_DIRECTORY ${CMAKE_BINARY_DIR}/bindings)

  if (HAVE_PERL_BINDINGS)
    add_subdirectory(perl)
  endif (HAVE_PERL_BINDINGS)

  if (HAVE_PYTHON_BINDINGS)
    add_subdirectory(python)
  endif (HAVE_PYTHON_BINDINGS)

  if (HAVE_RUBY_BINDINGS)
    add_subdirectory(ruby)
  endif (HAVE_RUBY_BINDINGS)

endif(HAVE_SWIG_BINDINGS)
