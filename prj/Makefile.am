bin_PROGRAMS = runner

CLEANFILES=tests/runner.cpp

tests/runner.cpp: ../common/t/layer_tags_tests.cpp
	cd tests; \
	perl cxxtestgen.pl --have-eh --runner=ErrorPrinter -o runner.cpp ../../common/t/layer_tags_tests.cpp

runner_SOURCES = \
	tests/runner.cpp

runner_CPPFLAGS = ${compiler_flags} \
    -Itests \
    -I../common \
    -I../common/t
