add_subdirectory(tokenizers)
add_subdirectory(lemmatizers)
add_subdirectory(parsers)

set(tools_DIRS 
	tokenizers/tp
	lemmatizers/morfologik
	parsers/puddle
CACHE PATH "Tools' paths to use in include_ and link_directories" FORCE
)

