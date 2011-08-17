add_library(common-test
	by_spaces_cutter.cpp
)
target_link_libraries(common-test common)

set(common-t
 	${psi-toolkit_SOURCE_DIR}/common/t/annotation_item_tests.hpp
	${psi-toolkit_SOURCE_DIR}/common/t/lattice_readers_writers_tests.hpp
	${psi-toolkit_SOURCE_DIR}/common/t/lattice_tests.hpp
	${psi-toolkit_SOURCE_DIR}/common/t/layer_tags_tests.hpp
	${psi-toolkit_SOURCE_DIR}/common/t/quoter_tests.hpp
CACHE PATH "CxxTests for common programms" FORCE)

if(USE_JAVA)

set(common-t 
	${common-t}
	${psi-toolkit_SOURCE_DIR}/common/t/java_virtual_machine_tests.hpp
CACHE PATH "CxxTests for common programms" FORCE)

endif(USE_JAVA)
