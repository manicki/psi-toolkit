#include "main_factories_keeper.hpp"

#include "config.hpp"

#include "txt_lattice_reader.hpp"
#include "utt_lattice_reader.hpp"
#include "psi_lattice_reader.hpp"
#include "psi_lattice_writer.hpp"
#include "simple_lattice_writer.hpp"
#include "json_lattice_writer.hpp"
#include "dot_lattice_writer.hpp"
#include "apertium_lattice_reader.hpp"
#include "nkjp_lattice_reader.hpp"
#include "tp_tokenizer.hpp"
#include "srx_segmenter.hpp"
#include "lemmatizer_annotator.hpp"
#include "lang_guesser.hpp"
#include "gobio.hpp"

#if HAVE_GRAPHVIZ
#include "gv_lattice_writer.hpp"
#endif

#if HAVE_POPPLER
#include "pdf_lattice_reader.hpp"
#endif

#if HAVE_POSTGRESQL
#include "lex_db_lemmatizer.hpp"
#endif
#if HAVE_JAVA
#include "morfologik.hpp"
#endif
#if HAVE_PUDDLE
#include "puddle.hpp"
#endif

#if HAVE_CMPH
#include "one_edge_at_a_time_annotator.hpp"
#include "bilexicon.hpp"
#endif

#if HAVE_PERL_BINDINGS
#include "perl_simple_lattice_writer.hpp"
#endif

#include "me_tagger.hpp"

MainFactoriesKeeper::MainFactoriesKeeper() {
    keeper_.takeProcessorFactory(new TxtLatticeReader::Factory());
    keeper_.takeProcessorFactory(new UTTLatticeReader::Factory());
    keeper_.takeProcessorFactory(new PsiLatticeReader::Factory());
    keeper_.takeProcessorFactory(new PsiLatticeWriter::Factory());
    keeper_.takeProcessorFactory(new SimpleLatticeWriter::Factory());
    keeper_.takeProcessorFactory(new JSONLatticeWriter::Factory());
    keeper_.takeProcessorFactory(new DotLatticeWriter::Factory());
    keeper_.takeProcessorFactory(new ApertiumLatticeReader::Factory());
    keeper_.takeProcessorFactory(new NKJPLatticeReader::Factory());
    keeper_.takeProcessorFactory(new TpTokenizer::Factory());
    keeper_.takeProcessorFactory(new SrxSegmenter::Factory());
    keeper_.takeProcessorFactory(new LangGuesser::Factory());
    keeper_.takeProcessorFactory(new Gobio::Factory());

#if HAVE_GRAPHVIZ
    keeper_.takeProcessorFactory(new GVLatticeWriter::Factory());
#endif

#if HAVE_POPPLER
    keeper_.takeProcessorFactory(new PDFLatticeReader::Factory());
#endif

#if HAVE_POSTGRESQL
    keeper_.takeProcessorFactory(new LemmatizerAnnotator<LexDbLemmatizer>::Factory());
#endif
#if HAVE_JAVA
    keeper_.takeProcessorFactory(new LemmatizerAnnotator<Morfologik>::Factory());
#endif
#if HAVE_PUDDLE
    keeper_.takeProcessorFactory(new poleng::bonsai::puddle::Puddle::Factory());
#endif

#if HAVE_CMPH
    keeper_.takeProcessorFactory(new OneEdgeAtATimeAnnotator<BiLexicon>::Factory());
#endif

#if HAVE_PERL_BINDINGS
    keeper_.takeProcessorFactory(new PerlSimpleLatticeWriter::Factory());
#endif
    keeper_.takeProcessorFactory(new MeTagger::Factory());
}

ProcessorFactory& MainFactoriesKeeper::getProcessorFactory(std::string processorName) {
    return keeper_.getProcessorFactory(processorName);
}

std::vector<std::string> MainFactoriesKeeper::getProcessorNames() {
    return keeper_.getProcessorNames();
}

MainFactoriesKeeper& MainFactoriesKeeper::getInstance() {
    static MainFactoriesKeeper instance;
    return instance;
}
