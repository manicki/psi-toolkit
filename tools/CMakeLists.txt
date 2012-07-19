add_subdirectory(tokenizers)
add_subdirectory(segmenters)
add_subdirectory(lemmatizers)
add_subdirectory(parsers)
add_subdirectory(datastructures)
add_subdirectory(recognizers)
add_subdirectory(lexica)
add_subdirectory(taggers)
add_subdirectory(spellcheckers)

set(tools_DIRS
    tokenizers/tp
    lemmatizers/morfologik
    parsers/gobio
    parsers/link
    parsers/puddle
    parsers/gobio
    parsers/gobio/avmatrix
    parsers/gobio/chart
    parsers/gobio/helpers
    parsers/gobio/parser
    parsers/gobio/registrar
    parsers/gobio/rules
    parsers/gobio/translator
    parsers/gobio/transporter
    parsers/gobio/trees
    segmenters/srx
    recognizers/langguesser
    lexica/bilexicon
    datastructures/lexica
    taggers/metagger
    spellcheckers/aspell
    CACHE PATH "Tools' paths to use in include_ and link_directories" FORCE
)


if(HAVE_SFST)
set(tools_DIRS
    ${tools_DIRS}
    lemmatizers/sfst CACHE PATH "Tools' paths to use in include_ and link_directories" FORCE)
endif(HAVE_SFST)
