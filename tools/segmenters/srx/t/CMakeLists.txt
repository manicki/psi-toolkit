
add_library(tools-segmenters-srx-tests
        srx_rules_tests.cpp
)

target_link_libraries(tools-segmenters-srx-tests
        tools-segmenters-srx
)

set(tools-segmenters-srx-t
	${psi-toolkit_SOURCE_DIR}/tools/segmenters/srx/t/srx_rules_tests.hpp
	CACHE PATH "CxxTests for TP tokenizer" FORCE
)
