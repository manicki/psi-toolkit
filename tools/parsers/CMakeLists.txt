add_subdirectory(puddle)
add_subdirectory(gobio)

if (HAVE_LINK_GRAMMAR)
    add_subdirectory(link)
endif (HAVE_LINK_GRAMMAR)
