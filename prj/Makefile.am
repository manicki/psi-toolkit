bin_PROGRAMS = runner psi-pipe puddle

cpprog=`echo $${CPPROG-cp -d -r}`

CLEANFILES=tests/runner.cpp

tests/runner.cpp: ../common/t/layer_tags_tests.hpp ../common/t/annotation_item_tests.hpp ../common/t/lattice_tests.hpp ../common/t/quoter_tests.hpp ../common/t/lattice_readers_writers_tests.hpp
	cd tests; \
	perl cxxtestgen.pl --have-eh --runner=ErrorPrinter -o runner.cpp ../../common/t/layer_tags_tests.hpp ../../common/t/annotation_item_tests.hpp ../../common/t/lattice_tests.hpp ../../common/t/quoter_tests.hpp ../../common/t/lattice_readers_writers_tests.hpp

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

puddle_SOURCES = \
	../tools/parsers/puddle/puddle_main.cpp \
    ../tools/parsers/puddle/action.cpp \
    ../tools/parsers/puddle/add_action.cpp \
    ../tools/parsers/puddle/attach_action.cpp \
    ../tools/parsers/puddle/delete_action.cpp \
    ../tools/parsers/puddle/entity.cpp \
    ../tools/parsers/puddle/graph_writer.cpp \
    ../tools/parsers/puddle/group_action.cpp \
    ../tools/parsers/puddle/group.cpp \
    ../tools/parsers/puddle/join_action.cpp \
    ../tools/parsers/puddle/parse_converter.cpp \
    ../tools/parsers/puddle/puddle.cpp \
    ../tools/parsers/puddle/rule.cpp \
    ../tools/parsers/puddle/rule_loader.cpp \
    ../tools/parsers/puddle/rule_matcher.cpp \
    ../tools/parsers/puddle/rule_modifier.cpp \
    ../tools/parsers/puddle/sentence_tagger.cpp \
    ../tools/parsers/puddle/special_token.cpp \
    ../tools/parsers/puddle/syntok_action.cpp \
    ../tools/parsers/puddle/syntok.cpp \
    ../tools/parsers/puddle/tagger.cpp \
    ../tools/parsers/puddle/tagset.cpp \
    ../tools/parsers/puddle/tagset_loader.cpp \
    ../tools/parsers/puddle/token.cpp \
    ../tools/parsers/puddle/transform_action.cpp \
    ../tools/parsers/puddle/unify_action.cpp

puddle_SOURCES += /home/leszekm/poleng/poleng4/src/bonsai/common/ParseGraph.cpp \
	/home/leszekm/poleng/poleng4/src/bonsai/common/TransitionInfo.cpp \
	/home/leszekm/poleng/poleng4/src/bonsai/common/TransferTypes.cpp

puddle_CPPFLAGS = ${compiler_flags} \
	-I/home/leszekm/poleng/poleng4/src/bonsai/common # -I/home/leszekm/poleng/poleng4/lib/linux/icu/include -I/home/leszekm/poleng/poleng4/lib/linux/boost/include/boost-1_37 -I/home/leszekm/poleng/poleng4/lib/linux/boost/include/boost-1_37/boost

LIBS += \
	-lre2 \
	-licuuc -licudata -licui18n -licuio \
	-lboost_graph -lboost_serialization -lboost_program_options -lboost_wserialization -lboost_regex #-L/home/leszekm/poleng/poleng4/lib/linux/icu/lib -L/home/leszekm/poleng/poleng4/lib/linux/boost/lib -lboost_graph-gcc44 -lboost_serialization-gcc44 -lboost_program_options-gcc44 -lboost_wserialization-gcc44 -lboost_regex-gcc44
