add_subdirectory(tokenizers)
add_subdirectory(segmenters)
add_subdirectory(lemmatizers)
add_subdirectory(parsers)
add_subdirectory(datastructures)

set(tools_DIRS
	tokenizers/tp
	lemmatizers/morfologik
	parsers/puddle
        segmenters/srx
CACHE PATH "Tools' paths to use in include_ and link_directories" FORCE
)

