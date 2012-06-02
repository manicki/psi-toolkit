
# don't want to see warnings about deprecated hash_maps/sets used
# in SFST
add_definitions(-Wno-deprecated -DSGI__gnu_cxx)

add_library(tools-lemmatizers-sfst SHARED
	sfst_lemmatizer.cpp
)

#target_link_libraries(sfst)

add_subdirectory(t)

install(TARGETS tools-lemmatizers-sfst DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/sfstLemmatizer)
