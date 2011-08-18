add_library(framework
	factories_keeper.cpp
	main_factories_keeper.cpp
	pipe_runner.cpp
)

foreach(dir ${formats_SUBDIRS})
	include_directories("${psi-toolkit_SOURCE_DIR}/formats/${dir}")
	link_directories("${psi-toolkit_BINARY_DIR}/formats/${dir}")
endforeach(dir)

foreach(dir ${tools_DIRS})
	include_directories("${psi-toolkit_SOURCE_DIR}/tools/${dir}")
	link_directories("${psi-toolkit_BINARY_DIR}/tools/${dir}")
endforeach(dir)

add_executable(psi-pipe psi-pipe.cpp)
target_link_libraries(psi-pipe framework ${Boost_LIBRARIES} common formats-psi formats-txt formats-utt tools-tokenizers-tp)

if(USE_POSTGRESQL)
        include_directories("${psi-toolkit_SOURCE_DIR}/tools/lemmatizers/lex_db")
        target_link_libraries(lex-db)
endif(USE_POSTGRESQL)

install(TARGETS psi-pipe DESTINATION "${psi-toolkit_BINARY_DIR}")
install(FILES DESTINATION "${psi-toolkit_BINARY_DIR}")

