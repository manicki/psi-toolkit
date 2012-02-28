add_library(tools-lemmatizers-sfst SHARED
	sfst_lemmatizer.cpp
)

install(TARGETS tools-lemmatizers-sfst DESTINATION lib/)

add_subdirectory(t)
