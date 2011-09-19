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


add_executable(
 mass-tests
 mass-tests.cpp
 mass_tester.cpp
 batch_runner.cpp
 junit_mass_tests_reporter.cpp
)

target_link_libraries(
 mass-tests
 ${BASE_TARGETS}
 ${Boost_LIBRARIES}
 pcre
 pcrecpp
)


