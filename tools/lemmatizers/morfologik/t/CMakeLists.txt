
add_library(morfologik-tests
        morfologik_tests.cpp
        morfologik_tags_parser_tests.cpp
)


target_link_libraries(morfologik-tests
  tools-lemmatizers-morfologik
  common-tests
)

set(morfologik-t
	${psi-toolkit_SOURCE_DIR}/tools/lemmatizers/morfologik/t/morfologik_tests.hpp
	${psi-toolkit_SOURCE_DIR}/tools/lemmatizers/morfologik/t/morfologik_tags_parser_tests.hpp
	CACHE PATH "CxxTests for morfologik lemmatizer" FORCE
)
