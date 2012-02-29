set(formats_SUBDIRS
    dot gv psi psi/psi_reader psi/psi_writer utt txt simple apertium apertium/apertium_reader
CACHE PATH "Formats subdirectories" FORCE)

foreach(dir ${formats_SUBDIRS})
    add_subdirectory(${dir})
endforeach(dir)

