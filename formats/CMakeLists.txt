set(formats_SUBDIRS
    apertium apertium/apertium_reader
    bracketing bracketing/bracketing-writer
    dot
    guessing guessing/guessing_reader
    nkjp
    psi psi/psi_reader psi/psi_writer
    simple json-simple
    txt
    utt
CACHE PATH "Formats subdirectories" FORCE)

if(HAVE_GRAPHVIZ)
    set(formats_SUBDIRS ${formats_SUBDIRS} gv)
else(HAVE_GRAPHVIZ)
    unset(gv-t CACHE)
endif(HAVE_GRAPHVIZ)

if(HAVE_POPPLER)
    set(formats_SUBDIRS ${formats_SUBDIRS} pdf)
else(HAVE_POPPLER)
    unset(pdf-t CACHE)
endif(HAVE_POPPLER)

if(HAVE_DJVULIBRE)
    set(formats_SUBDIRS ${formats_SUBDIRS} djvu)
else(HAVE_DJVULIBRE)
    unset(djvu-t CACHE)
endif(HAVE_DJVULIBRE)

if(USE_DOC_READER)
    set(formats_SUBDIRS ${formats_SUBDIRS} doc)
else(USE_DOC_READER)
    unset(doc-t CACHE)
endif(USE_DOC_READER)

foreach(dir ${formats_SUBDIRS})
    add_subdirectory(${dir})
endforeach(dir)
