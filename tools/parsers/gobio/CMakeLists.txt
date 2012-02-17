add_library(tools-parsers-gobio SHARED
    gobio.cpp
)

set(gobio_SUBDIRS
    chart parser rules
CACHE PATH "Gobio subdirectories" FORCE)

include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/chart")
include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/parser")
include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/rules")

add_subdirectory("chart")
add_subdirectory("parser")
add_subdirectory("rules")
add_subdirectory("t")

target_link_libraries(tools-parsers-gobio)

install(TARGETS tools-parsers-gobio DESTINATION lib/)
