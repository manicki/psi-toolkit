add_subdirectory(tokenizers)
add_subdirectory(segmenters)
add_subdirectory(lemmatizers)
add_subdirectory(parsers)
add_subdirectory(datastructures)
add_subdirectory(recognizers)
add_subdirectory(lexica)
add_subdirectory(taggers)

set(tools_DIRS
    tokenizers/tp
    lemmatizers/morfologik
    parsers/gobio
    parsers/puddle
    segmenters/srx
    recognizers/langguesser
    lexica/bilexicon
    datastructures/lexica
    taggers/metagger
    CACHE PATH "Tools' paths to use in include_ and link_directories" FORCE
)

