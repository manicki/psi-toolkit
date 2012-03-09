# library server
add_library(server SHARED
    psi_request_handler.cpp
    psi_server.cpp
    index_site.cpp
    pipe_site.cpp
    help_site.cpp
    json_site.cpp
    guid_generator.cpp
    session.cpp
    session_manager.cpp
    server_runner.cpp
    output_saver.cpp
    filetype_recognizer.cpp
)

target_link_libraries(server ${Boost_LIBRARIES} pthread mpfd-parser ${BASE_TARGETS} pcre pcrecpp tools-parsers-puddle test-extractor)
if (WITH_RE2)
    target_link_libraries(server re2)
endif(WITH_RE2)

if(HAVE_PERL_BINDINGS)
    target_link_libraries(server formats-perl-simple)
    find_package(PerlLibs)
endif(HAVE_PERL_BINDINGS)

set_target_properties(server PROPERTIES LINK_FLAGS "-lpthread")

include_directories("${psi-toolkit_SOURCE_DIR}/server/server")
link_directories("${psi-toolkit_BINARY_DIR}/server/server")

include_directories("${psi-toolkit_SOURCE_DIR}/server/mpfd-parser")
link_directories("${psi-toolkit_BINARY_DIR}/server/mpfd-parser")

include_directories("${psi-toolkit_SOURCE_DIR}/server")
link_directories("${psi-toolkit_BINARY_DIR}/server")

include_directories("${psi-toolkit_SOURCE_DIR}/tests")
link_directories("${psi-toolkit_BINARY_DIR}/tests")

# executable psi-server
add_executable(psi-server
    psi-server.cpp
)
target_link_libraries(psi-server ${Boost_LIBRARIES} server http-server)

install(TARGETS psi-server DESTINATION bin/)
install(FILES DESTINATION "${psi-toolkit_BINARY_DIR}")

add_subdirectory(mpfd-parser)
add_subdirectory(server)

install(TARGETS server DESTINATION lib/)
install(DIRECTORY website/ DESTINATION ${WEBSITE_ROOT})
