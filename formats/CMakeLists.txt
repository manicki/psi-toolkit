set(formats_SUBDIRS psi txt utt CACHE PATH "Formats subdirectories" FORCE)

foreach(dir ${formats_SUBDIRS})
	add_subdirectory(${dir})
endforeach(dir)

