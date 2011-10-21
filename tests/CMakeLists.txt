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
 ${BASE_TARGETS}
 pcre
 pcrecpp
)
if (WITH_RE2)
    target_link_libraries(test_runner re2)
endif(WITH_RE2)

if(HAVE_PERL_BINDINGS)
    target_link_libraries(test_runner formats-perl-simple)
    find_package(PerlLibs)
    include_directories(${PERL_INCLUDE_PATH})

endif(HAVE_PERL_BINDINGS)

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
if (WITH_RE2)
    target_link_libraries(mass-tests re2)
endif(WITH_RE2)

if(HAVE_PERL_BINDINGS)
    target_link_libraries(mass-tests formats-perl-simple)  
    find_package(PerlLibs)
    include_directories(${PERL_INCLUDE_PATH})
endif(HAVE_PERL_BINDINGS)

