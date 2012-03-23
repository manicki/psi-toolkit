set(formats_SUBDIRS
    apertium apertium/apertium_reader dot nkjp psi psi/psi_reader psi/psi_writer simple txt utt
CACHE PATH "Formats subdirectories" FORCE)

foreach(dir ${formats_SUBDIRS})
    add_subdirectory(${dir})
endforeach(dir)

if(HAVE_GRAPHVIZ)
  add_subdirectory(gv)
else(HAVE_GRAPHVIZ)
  unset(gv-t CACHE)
endif(HAVE_GRAPHVIZ)

if(HAVE_POPPLER)
  add_subdirectory(pdf)
else(HAVE_POPPLER)
  unset(pdf-t CACHE)
endif(HAVE_POPPLER)
