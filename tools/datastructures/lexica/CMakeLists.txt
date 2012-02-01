add_library(tools-datastructures-lexica SHARED
    murmur_hash3.cpp
    perfect_hash_index.cpp
)

target_link_libraries(tools-datastructures-lexica cmph)

add_subdirectory(t)
