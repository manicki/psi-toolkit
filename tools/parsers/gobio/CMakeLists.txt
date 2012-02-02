add_library(tools-parsers-gobio SHARED
    gobio.cpp
)

set(gobio_SUBDIRS
    chart parser
CACHE PATH "Gobio subdirectories" FORCE)

include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/chart")
include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/parser")

add_subdirectory("chart")
add_subdirectory("parser")

install(TARGETS tools-parsers-gobio DESTINATION lib/)
