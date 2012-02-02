include_directories("${psi-toolkit_SOURCE_DIR}/tools/datastructures/lexica")

add_library(tools-datastructures-lexica-tests
        test_monotonic_vector.cpp
        test_perfect_hash_index.cpp
        test_string_vector.cpp
)
