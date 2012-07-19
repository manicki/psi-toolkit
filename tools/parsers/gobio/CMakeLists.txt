add_library(tools-parsers-gobio SHARED
    gobio.cpp
    avmatrix/av_ai_converter.cpp
    registrar/number_master.cpp
    registrar/zvalue_master.cpp
    rules/compile_gruleexpression.cpp
    rules/grule_helpers.cpp
    translator/lex.grlex.cpp
    translator/grparser.cpp
    translator/grparser.bis.cpp
    translator/grules.cpp
    translator/prandseq.cpp
    transporter/semantics_wrapper.cpp
    transporter/semantics_wrapper_for_zvalue.cpp
)

set_source_files_properties(${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/translator/lex.grlex.cpp GENERATED)
set_source_files_properties(${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/translator/grparser.bis.cpp GENERATED)

set(gobio_SUBDIRS avmatrix chart helpers parser registrar rules translator transporter trees CACHE PATH "Gobio subdirectories" FORCE)

foreach(subdir ${gobio_SUBDIRS})
    include_directories("${psi-toolkit_SOURCE_DIR}/tools/parsers/gobio/${subdir}")
    link_directories("${psi-toolkit_BINARY_DIR}/tools/parsers/gobio/${subdir}")
    add_subdirectory(${subdir})
endforeach(subdir)

add_subdirectory("t")

target_link_libraries(tools-parsers-gobio)

install(TARGETS tools-parsers-gobio DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/gobio)
