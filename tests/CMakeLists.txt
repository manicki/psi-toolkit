add_executable(
 test_runner
 test_runner.cpp
)

target_link_libraries(
 test_runner
 -Wl,-whole-archive
 ${TESTS_TARGETS}
 -Wl,-no-whole-archive
 ${Boost_LIBRARIES}
 pcre
 pcrecpp
)


