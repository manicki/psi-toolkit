bin_PROGRAMS = runner

cpprog=`echo $${CPPROG-cp -d -r}`

CLEANFILES=tests/runner.cpp

tests/runner.cpp: ../common/t/lattice_tests.hpp ../common/t/layer_tags_tests.hpp ../common/t/utt_reader_tests.hpp
	cd tests; \
	perl cxxtestgen.pl --have-eh --runner=ErrorPrinter -o runner.cpp ../../common/t/lattice_tests.hpp ../../common/t/layer_tags_tests.hpp ../../common/t/utt_reader_tests.hpp

runner_SOURCES = \
	tests/runner.cpp \
	../common/annotation_item.cpp \
	../common/lattice.cpp \
	../common/layer_tag_collection.cpp \
	../common/layer_tag_manager.cpp \
	../common/utt_reader.cpp

runner_CPPFLAGS = ${compiler_flags} \
    -Itests \
    -I../common \
    -I../common/t

data_to_be_installed = install-testfiles

install-testfiles:
	mkdir -p $(exec_prefix)/test/files
	$(cpprog) ../formats/utt/t/files/*.* $(exec_prefix)/test/files

install-data-hook: $(data_to_be_installed)

