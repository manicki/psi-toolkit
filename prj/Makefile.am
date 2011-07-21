bin_PROGRAMS = runner

cpprog=`echo $${CPPROG-cp -d -r}`

CLEANFILES=tests/runner.cpp

tests/runner.cpp: ../common/t/layer_tags_tests.hpp ../common/t/annotation_item_tests.hpp ../common/t/lattice_tests.hpp ../common/t/quoter_tests.hpp ../common/t/lattice_readers_writers_tests.hpp
	cd tests; \
	perl cxxtestgen.pl --have-eh --runner=ErrorPrinter -o runner.cpp ../../common/t/layer_tags_tests.hpp ../../common/t/annotation_item_tests.hpp ../../common/t/lattice_tests.hpp ../../common/t/quoter_tests.hpp ../../common/t/lattice_readers_writers_tests.hpp

runner_SOURCES = \
	tests/runner.cpp \
	../common/annotation_item.cpp \
	../common/annotation_item_manager.cpp \
	../common/lattice.cpp \
	../common/lattice_reader.cpp \
	../common/lattice_reader_factory.cpp \
	../common/layer_tag_collection.cpp \
	../common/layer_tag_manager.cpp \
	../common/layer_tag_mask.cpp \
	../common/processor.cpp \
	../common/processor_factory.cpp \
	../common/quoter.cpp \
	../common/utf8_string.cpp \
	../common/utt_like_quoter.cpp \
	../formats/psi/psi_lattice_reader.cpp \
	../formats/psi/psi_lattice_writer.cpp \
	../formats/psi/psi_quoter.cpp \
	../formats/txt/txt_lattice_reader.cpp \
	../formats/utt/utt_lattice_reader.cpp \
	../formats/utt/utt_quoter.cpp

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
	mkdir -p $(exec_prefix)/formats/psi/t/files
	mkdir -p $(exec_prefix)/formats/utt/t/files
	$(cpprog) ../formats/psi/t/files/*.* $(exec_prefix)/formats/psi/t/files/
	$(cpprog) ../formats/utt/t/files/*.* $(exec_prefix)/formats/utt/t/files/

install-data-hook: $(data_to_be_installed)

