set(formats_SUBDIRS
    psi psi/psi_reader psi/psi_writer utt txt simple
CACHE PATH "Formats subdirectories" FORCE)

foreach(dir ${formats_SUBDIRS})
	add_subdirectory(${dir})
endforeach(dir)

