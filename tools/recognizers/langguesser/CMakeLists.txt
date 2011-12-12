add_library(tools-recognizers-langguesser SHARED
	lang_guesser.cpp
)
target_link_libraries(tools-recognizers-langguesser common ${Boost_LIBRARIES})

install(TARGETS tools-recognizers-langguesser DESTINATION lib/)
