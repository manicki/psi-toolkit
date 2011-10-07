add_library(tools-tokenizers-tp SHARED
	tp_token_cutter.cpp
	tp_tokenizer.cpp
        tp_basic_tokenizer_rule_set.cpp
)

target_link_libraries(
 tools-tokenizers-tp
 common
)

add_subdirectory(t)

install(TARGETS tools-tokenizers-tp DESTINATION "${psi-toolkit_BINARY_DIR}")
