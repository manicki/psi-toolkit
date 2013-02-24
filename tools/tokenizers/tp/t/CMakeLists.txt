get_filename_component(PARENT_DIR ".." ABSOLUTE)
include_directories("${PARENT_DIR}")

add_library(tools-tokenizers-tp-tests
        tp_basic_tokenizer_rule_set_tests.cpp
        lang_tokenizer_rules_tests.cpp
)

target_link_libraries(tools-tokenizers-tp-tests
  common
  tools-tokenizers-tp
)

