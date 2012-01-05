add_library(tools-recognizers-langguesser SHARED
    bigram_language_model.cpp
	lang_guesser.cpp
)
target_link_libraries(tools-recognizers-langguesser common ${Boost_LIBRARIES})

add_subdirectory(t)

install(TARGETS tools-recognizers-langguesser DESTINATION lib/)
install(DIRECTORY data/ DESTINATION ${PSITOOLKIT_INSTALL_DATA_DIR}/lang-guesser)

add_executable(bigram-generator bigram-generator.cpp)

include_directories("${psi-toolkit_SOURCE_DIR}/tools/recognizers/langguesser")
link_directories("${psi-toolkit_BINARY_DIR}/tools/recognizers/langguesser")
target_link_libraries(bigram-generator ${Boost_LIBRARIES} pcre pcrecpp common tools-recognizers-langguesser )

install(TARGETS bigram-generator DESTINATION bin/)
