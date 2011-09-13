
add_library(tools-tokenizers-tp-tests
        tp_basic_tokenizer_rule_set_tests.cpp
)

target_link_libraries(tools-tokenizers-tp-tests common tools-tokenizers-tp)

set(tools-tokenizers-tp-t
	${psi-toolkit_SOURCE_DIR}/tools/tokenizers/tp/t/tp_basic_tokenizer_rule_set_tests.hpp
	CACHE PATH "CxxTests for TP tokenizer" FORCE
)
