add_library(server
	psi_request_handler.cpp
	psi_server.cpp
)

target_link_libraries(server ${Boost_LIBRARIES} pthread)

set_target_properties(server PROPERTIES LINK_FLAGS "-lpthread")

include_directories("${psi-toolkit_SOURCE_DIR}/server/server")
link_directories("${psi-toolkit_BINARY_DIR}/server/server")

include_directories("${psi-toolkit_SOURCE_DIR}/server")
link_directories("${psi-toolkit_BINARY_DIR}/server")

add_executable(psi-server psi-server.cpp)
target_link_libraries(psi-server ${Boost_LIBRARIES} server http-server)

install(TARGETS psi-server DESTINATION "${psi-toolkit_BINARY_DIR}")
install(FILES DESTINATION "${psi-toolkit_BINARY_DIR}")

add_subdirectory(server)
