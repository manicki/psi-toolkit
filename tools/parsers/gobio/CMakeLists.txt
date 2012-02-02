add_library(tools-parsers-gobio SHARED
    gobio.cpp
)

set(gobio_SUBDIRS
    chart parser
CACHE PATH "Gobio subdirectories" FORCE)

foreach(dir ${gobio_SUBDIRS})
    add_subdirectory(${dir})
endforeach(dir)

install(TARGETS tools-parsers-gobio DESTINATION lib/)
