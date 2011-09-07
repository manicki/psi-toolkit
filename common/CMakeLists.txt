if(USE_JAVA)
	set(common-java-files
		java_virtual_machine.cpp
	)
endif(USE_JAVA)

add_library(common
	annotation_item.cpp
	annotation_item_manager.cpp
	annotator.cpp
	annotator_factory.cpp
        ant_like_path_glob.cpp
	cutter.cpp
	file_fetcher.cpp
	lattice.cpp
	lattice_reader.cpp
	lattice_reader_factory.cpp
	lattice_worker.cpp
	lattice_writer.cpp
	lattice_writer_factory.cpp
	layer_tag_collection.cpp
	layer_tag_manager.cpp
	layer_tag_mask.cpp
        lang_specific_processor_file_fetcher.cpp
	lemmatizer_output_iterator.cpp
	processor.cpp
	processor_factory.cpp
	processor_file_fetcher.cpp
	quoter.cpp
	reader_worker.cpp
        regexp.cpp
        segment_glob.cpp
        utt_like_quoter.cpp
	writer_worker.cpp
	${common-java-files}
)

target_link_libraries(common ${Boost_LIBRARIES})

if(USE_JAVA)
	target_link_libraries(common ${JAVA_JVM_LIBRARY})
	add_subdirectory(j)
endif(USE_JAVA)

add_subdirectory(t)



