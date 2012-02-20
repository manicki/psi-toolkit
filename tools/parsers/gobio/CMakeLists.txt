add_library(tools-parsers-gobio SHARED
    gobio.cpp
    registrar/number_master.cpp
    rules/compile_gruleexpression.cpp
    rules/grule_helpers.cpp
    translator/lex.grlex.cpp
    translator/grparser.cpp
    translator/grparser.bis.cpp
    translator/grules.cpp
    translator/prandseq.cpp
)

set_source_files_properties(${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/translator/lex.grlex.cpp GENERATED)
set_source_files_properties(${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/translator/grparser.bis.cpp GENERATED)

set(gobio_SUBDIRS
    avmatrix chart parser registrar rules translator trees
CACHE PATH "Gobio subdirectories" FORCE)

include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/avmatrix")
include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/chart")
include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/parser")
include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/registrar")
include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/rules")
include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/translator")
include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/trees")

add_subdirectory("avmatrix")
add_subdirectory("chart")
add_subdirectory("parser")
add_subdirectory("registrar")
add_subdirectory("rules")
add_subdirectory("translator")
add_subdirectory("trees")
add_subdirectory("t")

target_link_libraries(tools-parsers-gobio)

install(TARGETS tools-parsers-gobio DESTINATION lib/)
