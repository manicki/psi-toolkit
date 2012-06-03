
# don't want to see warnings about deprecated hash_maps/sets used
# in SFST
add_definitions(-Wno-deprecated -DSGI__gnu_cxx)

add_library(tools-lemmatizers-sfst SHARED
	sfst_lemmatizer.cpp
        sfst_plugin.cpp
)

target_link_libraries(
        tools-lemmatizers-sfst
        common
        common-plugins
)

add_subdirectory(t)
add_subdirectory(plugin)

install(TARGETS tools-lemmatizers-sfst DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/sfstLemmatizer)
