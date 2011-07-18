bin_PROGRAMS = runner

cpprog=`echo $${CPPROG-cp -d -r}`

CLEANFILES=tests/runner.cpp

tests/runner.cpp: ../common/t/layer_tags_tests.hpp ../common/t/lattice_tests.hpp ../common/t/lattice_readers_writers_tests.hpp
	cd tests; \
	perl cxxtestgen.pl --have-eh --runner=ErrorPrinter -o runner.cpp ../../common/t/layer_tags_tests.hpp ../../common/t/lattice_tests.hpp ../../common/t/lattice_readers_writers_tests.hpp

runner_SOURCES = \
	tests/runner.cpp \
	../common/annotation_item.cpp \
	../common/lattice.cpp \
	../common/lattice_rw_utils.cpp \
	../common/layer_tag_collection.cpp \
	../common/layer_tag_manager.cpp \
	../common/layer_tag_mask.cpp \
	../formats/psi/psi_lattice_reader.cpp \
	../formats/psi/psi_lattice_writer.cpp \
	../formats/utt/utt_lattice_reader.cpp

runner_CPPFLAGS = ${compiler_flags} \
    -Itests \
    -I../common \
    -I../common/t \
    -I../formats/psi \
    -I../formats/psi/t \
    -I../formats/utt \
    -I../formats/utt/t

data_to_be_installed = install-testfiles

install-testfiles:
	mkdir -p $(exec_prefix)/test/files/psi
	mkdir -p $(exec_prefix)/test/files/utt
	$(cpprog) ../formats/psi/t/files/*.* $(exec_prefix)/test/files/psi/
	$(cpprog) ../formats/utt/t/files/*.* $(exec_prefix)/test/files/utt/

install-data-hook: $(data_to_be_installed)

