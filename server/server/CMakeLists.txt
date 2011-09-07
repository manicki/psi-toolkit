add_library(http-server
	connection.cpp
	mime_types.cpp
	reply.cpp
	request_handler.cpp
	request_parser.cpp
	server.cpp
)

target_link_libraries(http-server ${Boost_LIBRARIES})
