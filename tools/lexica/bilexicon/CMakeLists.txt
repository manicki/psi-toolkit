include_directories("${psi-toolkit_SOURCE_DIR}/tools/datastructures/lexica")

add_library(tools-lexica-bilexicon SHARED
        bilexicon.cpp
)

target_link_libraries(tools-lexica-bilexicon
        tools-datastructures-lexica
)


add_subdirectory(t)

install(TARGETS tools-lexica-bilexicon DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/bilexicon)
