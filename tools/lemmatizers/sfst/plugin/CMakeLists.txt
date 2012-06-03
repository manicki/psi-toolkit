# don't want to see warnings about deprecated hash_maps/sets used
# in SFST
add_definitions(-Wno-deprecated -DSGI__gnu_cxx)

add_library(tools-lemmatizers-sfst-plugin SHARED
	sfst_adapter_impl.cpp
)

target_link_libraries(
  tools-lemmatizers-sfst-plugin
  sfst
)

install(TARGETS tools-lemmatizers-sfst-plugin DESTINATION lib/)
