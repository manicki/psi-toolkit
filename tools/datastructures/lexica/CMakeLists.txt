add_library(tools-datastructures-lexica SHARED
    key_value_store.cpp
    lexicon_base.cpp
    murmur_hash3.cpp
    perfect_hash_index.cpp
)

target_link_libraries(tools-datastructures-lexica cmph)

add_subdirectory(t)

install(TARGETS tools-datastructures-lexica DESTINATION lib/)
