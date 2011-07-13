bin_PROGRAMS = runner

CLEANFILES=tests/runner.cpp

tests/runner.cpp: ../common/t/lattice_tests.hpp ../common/t/layer_tags_tests.hpp
	cd tests; \
	perl cxxtestgen.pl --have-eh --runner=ErrorPrinter -o runner.cpp ../../common/t/lattice_tests.hpp ../../common/t/layer_tags_tests.hpp

runner_SOURCES = \
	tests/runner.cpp \
	../common/annotation_item.cpp \
	../common/lattice.cpp \
	../common/layer_tag_collection.cpp \
	../common/layer_tag_manager.cpp

runner_CPPFLAGS = ${compiler_flags} \
    -Itests \
    -I../common \
    -I../common/t
