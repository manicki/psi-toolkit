add_library(puddle-tests
        puddle_tests.cpp
)

target_link_libraries(puddle-tests
  tools-parsers-puddle
)

set(puddle-t
	${psi-toolkit_SOURCE_DIR}/tools/parsers/puddle/t/puddle_tests.hpp
	CACHE PATH "CxxTests for puddle parser" FORCE
)
