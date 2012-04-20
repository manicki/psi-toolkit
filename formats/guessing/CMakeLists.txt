add_library(formats-guessing SHARED
    guessing_reader/guessing_reader.cpp
)

target_link_libraries(formats-guessing common formats-txt)

foreach(dir ${formats_SUBDIRS})
    include_directories("${psi-toolkit_SOURCE_DIR}/formats/${dir}")
    link_directories("${psi-toolkit_BINARY_DIR}/formats/${dir}")
endforeach(dir)

install(TARGETS formats-guessing DESTINATION lib/)
install(DIRECTORY guessing_reader/data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/guessing-reader)
