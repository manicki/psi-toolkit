add_library(tools-parsers-gobio SHARED
    gobio.cpp
)

set(gobio_SUBDIRS chart parser CACHE PATH "Gobio subdirectories" FORCE)

foreach(subdir ${gobio_SUBDIRS})
    include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/${subdir}")
    link_directories("${psi-toolkit_BINARY_DIR}/tools/parsers/gobio/${subdir}")
    add_subdirectory(${subdir})
endforeach(subdir)

add_subdirectory("t")

target_link_libraries(tools-parsers-gobio)

install(TARGETS tools-parsers-gobio DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/gobio)
