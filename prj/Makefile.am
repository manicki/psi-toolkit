bin_PROGRAMS = runner psi-pipe
if WITH_PUDDLE
bin_PROGRAMS += puddle
endif

cpprog=`echo $${CPPROG-cp -d -r}`

CLEANFILES=tests/runner.cpp

# Only one place where the .hpp test files have to be specified
#test_files = ../common/t/layer_tags_tests.hpp ../common/t/annotation_item_tests.hpp ../common/t/lattice_tests.hpp ../common/t/quoter_tests.hpp ../common/t/lattice_readers_writers_tests.hpp
test_files = ../common/t/layer_tags_tests.hpp ../common/t/annotation_item_tests.hpp ../common/t/lattice_tests.hpp ../common/t/quoter_tests.hpp ../common/t/lattice_readers_writers_tests.hpp

if WITH_JAVA
test_files += ../common/t/java_virtual_machine_tests.hpp ../tools/lemmatizers/morfologik/t/morfologik_tests.hpp ../tools/lemmatizers/morfologik/t/morfologik_tags_parser_tests.hpp
endif

if WITH_PUDDLE
test_files += ../tools/parsers/puddle/t/puddle_tests.hpp
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
	../tools/lemmatizers/morfologik/morfologik_tags_parser.cpp \
	../tools/lemmatizers/morfologik/morfologik.cpp
endif

if WITH_PUDDLE
runner_SOURCES += \
	../tools/parsers/puddle/puddle.cpp \
	../tools/parsers/puddle/tagset_loader.cpp \
	../tools/parsers/puddle/rule_loader.cpp \
	../tools/parsers/puddle/tagger.cpp \
	../tools/parsers/puddle/tagset.cpp \
	../tools/parsers/puddle/action.cpp \
	../tools/parsers/puddle/add_action.cpp \
	../tools/parsers/puddle/attach_action.cpp \
	../tools/parsers/puddle/delete_action.cpp \
	../tools/parsers/puddle/group_action.cpp \
	../tools/parsers/puddle/join_action.cpp \
	../tools/parsers/puddle/syntok_action.cpp \
	../tools/parsers/puddle/transform_action.cpp \
	../tools/parsers/puddle/unify_action.cpp \
	../tools/parsers/puddle/puddle_util.cpp \
	../tools/parsers/puddle/rule.cpp \
	../tools/parsers/puddle/rule_matcher.cpp \
	../tools/parsers/puddle/rule_modifier.cpp \
	../tools/parsers/puddle/lattice_wrapper.cpp
if WITH_BONSAI_PARSEGRAPH
runner_SOURCES += \
	@BONSAI_PATH@/ParseGraph.cpp \
	@BONSAI_PATH@/TransferTypes.cpp \
	@BONSAI_PATH@/TransitionInfo.cpp
endif

#	../tools/parsers/puddle/graph_writer.cpp
#	../tools/parsers/puddle/parse_converter.cpp

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
if WITH_PUDDLE
runner_CPPFLAGS += -I../tools/parsers/puddle
endif

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

if WITH_PUDDLE
puddle_SOURCES = \
	../tools/parsers/puddle/puddle_main.cpp \
    ../tools/parsers/puddle/action.cpp \
    ../tools/parsers/puddle/add_action.cpp \
    ../tools/parsers/puddle/attach_action.cpp \
    ../tools/parsers/puddle/delete_action.cpp \
    ../tools/parsers/puddle/group_action.cpp \
    ../tools/parsers/puddle/join_action.cpp \
	../tools/parsers/puddle/lattice_wrapper.cpp \
    ../tools/parsers/puddle/puddle.cpp \
	../tools/parsers/puddle/puddle_util.cpp \
    ../tools/parsers/puddle/rule.cpp \
    ../tools/parsers/puddle/rule_loader.cpp \
    ../tools/parsers/puddle/rule_matcher.cpp \
    ../tools/parsers/puddle/rule_modifier.cpp \
    ../tools/parsers/puddle/syntok_action.cpp \
    ../tools/parsers/puddle/tagger.cpp \
    ../tools/parsers/puddle/tagset.cpp \
    ../tools/parsers/puddle/tagset_loader.cpp \
    ../tools/parsers/puddle/transform_action.cpp \
    ../tools/parsers/puddle/unify_action.cpp \
	../common/annotation_item.cpp \
	../common/annotation_item_manager.cpp \
	../common/layer_tag_collection.cpp \
	../common/layer_tag_manager.cpp \
	../common/layer_tag_mask.cpp \
	../common/cutter.cpp \
	../common/lattice.cpp
if WITH_BONSAI_PARSEGRAPH
puddle_SOURCES += \
	@BONSAI_PATH@/ParseGraph.cpp \
	@BONSAI_PATH@/TransitionInfo.cpp \
	@BONSAI_PATH@/TransferTypes.cpp
endif

#    ../tools/parsers/puddle/sentence_tagger.cpp
#    ../tools/parsers/puddle/graph_writer.cpp
#    ../tools/parsers/puddle/parse_converter.cpp

puddle_CPPFLAGS = ${compiler_flags} \
	-I../common \
	-I../utf8 \
	-I../utf8/utf8

endif
