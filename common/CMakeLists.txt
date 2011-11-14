if(USE_JAVA)
    set(common-java-files
        java_virtual_machine.cpp
    )
endif(USE_JAVA)

add_library(common SHARED
    aligning_writer_worker.cpp
    annotation_item.cpp
    annotation_item_manager.cpp
    annotator.cpp
    annotator_factory.cpp
    ant_like_path_glob.cpp
    configurator.cpp
    cutter.cpp
    escaping.cpp
    file_fetcher.cpp
    language_dependent_annotator_factory.cpp
    lattice.cpp
    lattice_iter_writer.cpp
    lattice_worker.cpp
    lattice_writer_output_iterator.cpp
    layer_tag_collection.cpp
    layer_tag_manager.cpp
    layer_tag_mask.cpp
    lang_specific_processor_file_fetcher.cpp
    lemmatizer_output_iterator.cpp
    object_cache.cpp
    processor.cpp
    processor_factory.cpp
    processor_file_fetcher.cpp
    psi_exception.cpp
    psi_quoter.cpp
    quoter.cpp
    regexp.cpp
    segment_glob.cpp
    stream_lattice_reader.cpp
    string_frag.cpp
    utt_like_quoter.cpp
    xml_property_tree.cpp
    ${common-java-files}
)

if(USE_JAVA)
    target_link_libraries(common ${JAVA_JVM_LIBRARY})
    add_subdirectory(j)
endif(USE_JAVA)

add_subdirectory(t)

install(TARGETS common DESTINATION lib/)
