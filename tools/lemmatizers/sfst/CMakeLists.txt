add_library(tools-lemmatizers-sfst SHARED
	sfst_lemmatizer.cpp
)

#target_link_libraries(sfst)

add_subdirectory(t)

install(TARGETS tools-lemmatizers-sfst DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/sfstLemmatizer)
