set(formats_SUBDIRS
    apertium apertium/apertium_reader 
    dot 
    guessing guessing/guessing_reader 
    nkjp 
    psi psi/psi_reader psi/psi_writer 
    simple json-simple 
    txt 
    utt
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

if(HAVE_DJVULIBRE)
  add_subdirectory(djvu)
else(HAVE_DJVULIBRE)
  unset(djvu-t CACHE)
endif(HAVE_DJVULIBRE)

if(USE_DOC_READER)
  add_subdirectory(doc)
else(USE_DOC_READER)
  unset(doc-t CACHE)
endif(USE_DOC_READER)

