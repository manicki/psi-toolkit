include_directories(
    "${psi-toolkit_SOURCE_DIR}/tools/lemmatizers/hfst/parsers"
    "${psi-toolkit_SOURCE_DIR}/tools/lemmatizers/hfst/stemmers"
    "${psi-toolkit_SOURCE_DIR}/tools/lemmatizers/hfst/plugin"
    "${psi-toolkit_SOURCE_DIR}/tools/lemmatizers/hfst"
)

add_library(tools-lemmatizers-hfst SHARED
    hfst_lemmatizer.cpp
    hfst_plugin.cpp
    parsers/hfst_english_tags_parser.cpp
    parsers/hfst_finnish_tags_parser.cpp
    parsers/hfst_french_tags_parser.cpp
    parsers/hfst_german_tags_parser.cpp
    parsers/hfst_italian_tags_parser.cpp
    parsers/hfst_swedish_tags_parser.cpp
    parsers/hfst_turkish_tags_parser.cpp
    stemmers/hfst_english_stemmer.cpp
    stemmers/hfst_finnish_stemmer.cpp
    stemmers/hfst_french_stemmer.cpp
    stemmers/hfst_german_stemmer.cpp
    stemmers/hfst_italian_stemmer.cpp
    stemmers/hfst_swedish_stemmer.cpp
    stemmers/hfst_turkish_stemmer.cpp
)

target_link_libraries(
    tools-lemmatizers-hfst
    common 
    common-plugins
)

add_subdirectory(t)
add_subdirectory(parsers)
add_subdirectory(stemmers)
add_subdirectory(plugin)

install(TARGETS tools-lemmatizers-hfst DESTINATION lib/)

install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/hfstLemmatizer)
