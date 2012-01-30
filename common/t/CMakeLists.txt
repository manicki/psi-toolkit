
set(COMMON_TESTS_EXTRAS)

if(USE_JAVA)
        set(COMMON_TESTS_EXTRAS ${COMMON_TESTS_EXTRAS} java_virtual_machine_tests.cpp)
endif(USE_JAVA)

include_directories("${psi-toolkit_SOURCE_DIR}/formats/simple")

add_library(common-tests
    by_spaces_cutter.cpp
    fake_lemmatizer.cpp
        segment_glob_tests.cpp
        annotation_item_tests.cpp
        ant_like_path_glob_tests.cpp
        file_fetcher_tests.cpp
        lattice_iter_writer_tests.cpp
        lattice_tests.cpp
        layer_tags_tests.cpp
        object_cache_tests.cpp
        quoter_tests.cpp
        regexp_tests.cpp
        string_frag_tests.cpp
        writers_tests_utils.cpp
        xml_property_tree_tests.cpp
        ${COMMON_TESTS_EXTRAS}
)


target_link_libraries(common-tests common)



