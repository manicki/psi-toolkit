add_library(tools-lemmatizers-morfologik SHARED
	morfologik_tags_parser.cpp
	morfologik.cpp
)
target_link_libraries(tools-lemmatizers-morfologik common ${JAVA_JVM_LIBRARY})

add_subdirectory(t)

install(TARGETS tools-lemmatizers-morfologik DESTINATION lib/)
install(FILES j/morfologik-stemming-1.5.0.jar DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/morfologik/)
