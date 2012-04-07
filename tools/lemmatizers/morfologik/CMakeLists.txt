add_library(tools-lemmatizers-morfologik SHARED
    morfologik.cpp
)
target_link_libraries(tools-lemmatizers-morfologik common ${JAVA_JVM_LIBRARY})

add_subdirectory(t)

install(TARGETS tools-lemmatizers-morfologik DESTINATION lib/)
install(FILES j/morfologik-stemming-1.5.2.jar DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/morfologik/)
install(FILES j/morfologik-polish-1.5.2.jar DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/morfologik/)
install(FILES j/morfologik-fsa-1.5.2.jar DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/morfologik/)
install(FILES j/hppc-0.4.0.jar DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/morfologik/)

install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/morfologik)
