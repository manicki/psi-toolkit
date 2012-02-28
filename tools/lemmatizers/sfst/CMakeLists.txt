add_library(tools-lemmatizers-sfst SHARED
	sfst_lemmatizer.cpp
)

add_subdirectory(t)

install(TARGETS tools-lemmatizers-sfst DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/sfstLemmatizer)
