include_directories("..")

add_library(tools-tokenizers-tp-tests
        tp_basic_tokenizer_rule_set_tests.cpp
)

target_link_libraries(tools-tokenizers-tp-tests
  common
  tools-tokenizers-tp
)

