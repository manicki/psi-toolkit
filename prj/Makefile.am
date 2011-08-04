bin_PROGRAMS = runner psi-pipe

cpprog=`echo $${CPPROG-cp -d -r}`

CLEANFILES=tests/runner.cpp

# Only one place where the .hpp test files have to be specified
test_files = ../common/t/layer_tags_tests.hpp ../common/t/annotation_item_tests.hpp ../common/t/lattice_tests.hpp ../common/t/quoter_tests.hpp ../common/t/lattice_readers_writers_tests.hpp

if WITH_JAVA
test_files += ../common/t/java_virtual_machine_tests.hpp ../common/t/morfologik_tests.hpp
endif

tests/runner.cpp: $(test_files)
	cd tests; \
	perl cxxtestgen.pl --have-eh --runner=ErrorPrinter -o runner.cpp $(foreach file, $(test_files), "../$(file)" )

# Should be in the WITH_JAVA condition but a bug occurs
JAVAROOT=../common/java

if WITH_JAVA
java_JAVA = \
	../common/java/HelloWorld.java
endif

runner_SOURCES = \
	tests/runner.cpp \
	../common/annotator.cpp \
	../common/annotator_factory.cpp \
	../common/annotation_item.cpp \
	../common/annotation_item_manager.cpp \
	../common/cutter.cpp \
	../common/lattice.cpp \
	../common/lattice_reader.cpp \
	../common/lattice_reader_factory.cpp \
	../common/lattice_worker.cpp \
	../common/lattice_writer.cpp \
	../common/lattice_writer_factory.cpp \
	../common/layer_tag_collection.cpp \
	../common/layer_tag_manager.cpp \
	../common/layer_tag_mask.cpp \
	../common/processor.cpp \
	../common/processor_factory.cpp \
	../common/quoter.cpp \
	../common/reader_worker.cpp \
	../common/utf8_string.cpp \
	../common/utt_like_quoter.cpp \
	../common/writer_worker.cpp \
	../common/t/by_spaces_cutter.cpp \
	../formats/psi/psi_lattice_reader.cpp \
	../formats/psi/psi_lattice_writer.cpp \
	../formats/psi/psi_quoter.cpp \
	../formats/txt/txt_lattice_reader.cpp \
	../formats/utt/utt_lattice_reader.cpp \
	../formats/utt/utt_quoter.cpp \
	../tools/tokenizers/tp/tp_tokenizer.cpp \
	../tools/tokenizers/tp/tp_token_cutter.cpp 

if WITH_JAVA
runner_SOURCES += \
	../common/java_virtual_machine.cpp \
	../tools/lemmatizers/morfologik/morfologik.cpp
endif

runner_CPPFLAGS = ${compiler_flags} \
	-Itests \
	-I../common \
	-I../common/t \
	-I../formats/psi \
	-I../formats/psi/t \
	-I../formats/utt \
	-I../formats/utt/t \
	-I../tools/tokenizers/tp \
	-I../utf8 \
	-I../utf8/utf8 

psi_pipe_SOURCES = \
	../common/annotator.cpp \
	../common/annotator_factory.cpp \
	../common/annotation_item.cpp \
	../common/annotation_item_manager.cpp \
	../common/cutter.cpp \
	../common/lattice.cpp \
	../common/lattice_reader.cpp \
	../common/lattice_reader_factory.cpp \
	../common/lattice_worker.cpp \
	../common/lattice_writer.cpp \
	../common/lattice_writer_factory.cpp \
	../common/layer_tag_collection.cpp \
	../common/layer_tag_manager.cpp \
	../common/layer_tag_mask.cpp \
	../common/processor.cpp \
	../common/processor_factory.cpp \
	../common/quoter.cpp \
	../common/reader_worker.cpp \
	../common/utf8_string.cpp \
	../common/utt_like_quoter.cpp \
	../common/writer_worker.cpp \
	../formats/psi/psi_lattice_reader.cpp \
	../formats/psi/psi_lattice_writer.cpp \
	../formats/psi/psi_quoter.cpp \
	../formats/txt/txt_lattice_reader.cpp \
	../formats/utt/utt_lattice_reader.cpp \
	../formats/utt/utt_quoter.cpp \
	../framework/factories_keeper.cpp \
	../framework/main_factories_keeper.cpp \
	../framework/pipe_runner.cpp \
	../framework/psi-pipe.cpp \
	../tools/tokenizers/tp/tp_tokenizer.cpp \
	../tools/tokenizers/tp/tp_token_cutter.cpp

psi_pipe_CPPFLAGS = ${compiler_flags} \
    -I../common \
    -I../formats/psi \
    -I../formats/txt \
    -I../formats/utt \
    -I../tools/tokenizers/tp \
    -I../utf8 \
    -I../utf8/utf8

data_to_be_installed = install-testfiles

install-testfiles:
	mkdir -p $(exec_prefix)/formats/psi/t/files
	mkdir -p $(exec_prefix)/formats/utt/t/files
	$(cpprog) ../formats/psi/t/files/*.* $(exec_prefix)/formats/psi/t/files/
	$(cpprog) ../formats/utt/t/files/*.* $(exec_prefix)/formats/utt/t/files/

install-data-hook: $(data_to_be_installed)

