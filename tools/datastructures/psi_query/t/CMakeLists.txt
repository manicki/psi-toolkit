include_directories("${psi-toolkit_SOURCE_DIR}/tools/datastructures/psi_query")

add_library(tools-datastructures-psi_query-tests
        test.cpp
	test_algorithms.cpp
	test_ndfsa.cpp
	test_epsRemove.cpp
)

