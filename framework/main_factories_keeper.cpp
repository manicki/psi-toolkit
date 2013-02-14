#include "main_factories_keeper.hpp"

#include "config.hpp"

#include "txt_lattice_reader.hpp"
#include "utt_lattice_reader.hpp"
#include "psi_lattice_reader.hpp"
#include "psi_lattice_writer.hpp"
#include "simple_lattice_writer.hpp"
#include "bracketing_lattice_writer.hpp"
#include "json_lattice_writer.hpp"
#include "dot_lattice_writer.hpp"
#include "apertium_lattice_reader.hpp"
#include "nkjp_lattice_reader.hpp"
#include "tp_tokenizer.hpp"
#include "srx_segmenter.hpp"
#include "lemmatizer_annotator.hpp"
#include "lang_guesser.hpp"
#include "gobio.hpp"
#include "puddle.hpp"

#if HAVE_GRAPHVIZ
#include "gv_lattice_writer.hpp"
#endif

#if HAVE_POPPLER
#include "pdf_lattice_reader.hpp"
#endif

#if HAVE_DJVULIBRE
#include "djvu_lattice_reader.hpp"
#endif

#if USE_DOC_READER
#include "doc_lattice_reader.hpp"
#endif

#if HAVE_POSTGRESQL
#include "lex_db_lemmatizer.hpp"
#endif

#if HAVE_JAVA
#include "morfologik.hpp"
#endif

#if HAVE_SFST
#include "sfst_lemmatizer.hpp"
#endif

#if HAVE_HFST
#include "hfst_lemmatizer.hpp"
#endif

#if HAVE_CMPH
#include "one_edge_at_a_time_annotator.hpp"
#include "bilexicon.hpp"
#endif

#if HAVE_PERL_BINDINGS
#include "perl_simple_lattice_writer.hpp"
#endif

#include "me_tagger.hpp"

#if HAVE_ASPELL
#include "psi_aspell.hpp"
#endif

#if HAVE_LIBMAGIC
#include "guessing_reader.hpp"
#endif

#if HAVE_LINK_GRAMMAR
#include "link_parser.hpp"
#endif

#include "bonsai.hpp"


MainFactoriesKeeper::MainFactoriesKeeper() {
    keeper_.addTagBasedIzeAliases("token", "token");
    keeper_.addTagBasedAlias("segment", "segment");
    keeper_.addTagBasedAlias("segment", "segmenter");
    keeper_.addTagBasedAlias("parse", "parse");
    keeper_.addTagBasedAlias("parse", "parser");
    keeper_.addTagBasedIzeAliases("lemma", "lemmat");

    keeper_.addVoidAlias("spell");
    keeper_.addAlias("spell-checker", "spell");
    keeper_.addAlias("spell-check", "spell");
    keeper_.addAlias("spellchecker", "spell");
    keeper_.addAlias("spellcheck", "spell");

#if HAVE_LIBMAGIC
    keeper_.addAlias("guess-input", "guessing-reader");
    keeper_.addAlias("guess-format", "guessing-reader");
#endif

    keeper_.addAlias("guess-lang", "lang-guesser");
    keeper_.addAlias("guess-language", "lang-guesser");

    keeper_.addAlias("read", "txt-reader");
    keeper_.addAlias("read-txt", "txt-reader");
    keeper_.addAlias("read-text", "txt-reader");
    keeper_.addAlias("text-reader", "txt-reader");

#if HAVE_POPPLER
    keeper_.addAlias("read-pdf", "pdf-reader");
#endif

#if HAVE_DJVULIBRE
    keeper_.addAlias("read-djvu", "djvu-reader");
#endif

#if USE_DOC_READER
    keeper_.addAlias("read-doc", "doc-reader");
#endif

    keeper_.addAlias("read-html", "apertium-reader");

    keeper_.addAlias("read-nkjp", "nkjp-reader");

    keeper_.addAlias("read-utt", "utt-reader");

    keeper_.addAlias("read-psi", "psi-reader");
    keeper_.addAlias("read-lattice", "psi-reader");

    keeper_.addAlias("write-psi", "psi-writer");
    keeper_.addAlias("write-lattice", "psi-writer");

    keeper_.addAlias("write", "simple-writer");
    keeper_.addAlias("write-simple", "simple-writer");

#if HAVE_GRAPHVIZ
    keeper_.addAlias("write-graph", "gv-writer");
    keeper_.addAlias("write-chart", "gv-writer");
    keeper_.addAlias("graph", "gv-writer");
    keeper_.addAlias("draw", "gv-writer");
#endif

    keeper_.addAlias("write-dot", "dot-writer");

    keeper_.addAlias("write-simple-json", "json-simple-writer");

    keeper_.takeProcessorFactory(new TxtLatticeReader::Factory());
    keeper_.takeProcessorFactory(new UTTLatticeReader::Factory());
    keeper_.takeProcessorFactory(new PsiLatticeReader::Factory());
    keeper_.takeProcessorFactory(new PsiLatticeWriter::Factory());
    keeper_.takeProcessorFactory(new SimpleLatticeWriter::Factory());
    keeper_.takeProcessorFactory(new BracketingLatticeWriter::Factory());
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

#if HAVE_DJVULIBRE
    keeper_.takeProcessorFactory(new DjVuLatticeReader::Factory());
#endif

#if USE_DOC_READER
    keeper_.takeProcessorFactory(new DocLatticeReader::Factory());
#endif

#if HAVE_POSTGRESQL
    keeper_.takeProcessorFactory(new LemmatizerAnnotator<LexDbLemmatizer>::Factory());
#endif
#if HAVE_JAVA
    keeper_.takeProcessorFactory(new LemmatizerAnnotator<Morfologik>::Factory());
#endif

#if HAVE_SFST
    keeper_.takeProcessorFactory(new LemmatizerAnnotator<SfstLemmatizer>::Factory());
#endif

#if HAVE_HFST
    keeper_.takeProcessorFactory(new LemmatizerAnnotator<HfstLemmatizer>::Factory());
#endif

    keeper_.takeProcessorFactory(new poleng::bonsai::puddle::Puddle::Factory());

#if HAVE_CMPH
    keeper_.takeProcessorFactory(new OneEdgeAtATimeAnnotator<BiLexicon>::Factory());
#endif

#if HAVE_PERL_BINDINGS
    keeper_.takeProcessorFactory(new PerlSimpleLatticeWriter::Factory());
#endif
    keeper_.takeProcessorFactory(new MeTagger::Factory());

#if HAVE_ASPELL
    keeper_.takeProcessorFactory(new PSIAspell::Factory());
#endif

#if HAVE_LIBMAGIC
    keeper_.takeProcessorFactory(new GuessingReader::Factory());
#endif

#if HAVE_LINK_GRAMMAR
    keeper_.takeProcessorFactory(new LinkParser::Factory());
#endif

    keeper_.takeProcessorFactory(new Bonsai::Factory());
}

ProcessorFactory& MainFactoriesKeeper::getProcessorFactory(std::string processorName) {
    return keeper_.getProcessorFactory(processorName);
}

std::vector<std::string> MainFactoriesKeeper::getProcessorNames() {
    return keeper_.getProcessorNames();
}

std::set<std::string> MainFactoriesKeeper::getAliasNames() {
    return keeper_.getAliasNames();
}
std::set<std::string> MainFactoriesKeeper::getAllAliases(std::string destination) {
    return keeper_.getAllAliases(destination);
}

std::list<ProcessorFactory*> MainFactoriesKeeper::getProcessorFactoriesForName(std::string name) {
    return keeper_.getProcessorFactoriesForName(name);
}

MainFactoriesKeeper& MainFactoriesKeeper::getInstance() {
    static MainFactoriesKeeper instance;
    return instance;
}
