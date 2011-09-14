
#!development,testing/500
# Tests of a given module should be compiled into a separate library,
# i.e. a `CMakeLists.txt` should created in the `t/` directory.
# For an example (and the template) of such of a file see <this file>.

# Library name should end with `-tests`.
add_library(tools-fooers-foo-tests
# Files with tests:
        foo_ying_tests.cpp
        foo_yang_tests.cpp
)

# The libraries that the tests depend on.
target_link_libraries(tools-fooers-foo-tests
  common
  tools-fooers-foo
)

