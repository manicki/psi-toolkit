add_library(tools-recognizers-langguesser SHARED
    bigram_language_model.cpp
	lang_guesser.cpp
)
target_link_libraries(tools-recognizers-langguesser common ${Boost_LIBRARIES})

add_subdirectory(t)

install(TARGETS tools-recognizers-langguesser DESTINATION lib/)
