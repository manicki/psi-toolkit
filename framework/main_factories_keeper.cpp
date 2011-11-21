#include "main_factories_keeper.hpp"

#include "config.h"

#include "txt_lattice_reader.hpp"
#include "utt_lattice_reader.hpp"
#include "psi_lattice_reader.hpp"
#include "psi_lattice_writer.hpp"
#include "simple_lattice_writer.hpp"
#include "tp_tokenizer.hpp"
#include "srx_segmenter.hpp"
#include "lemmatizer_annotator.hpp"

#if HAVE_POSTGRESQL
#include "lex_db_lemmatizer.hpp"
#endif
#if HAVE_JAVA
#include "morfologik.hpp"
#endif
#if HAVE_PUDDLE
#include "puddle.hpp"
#endif

#if HAVE_PERL_BINDINGS
#include "perl_simple_lattice_writer.hpp"
#endif

MainFactoriesKeeper::MainFactoriesKeeper() {
    keeper_.takeProcessorFactory(new TxtLatticeReader::Factory());
    keeper_.takeProcessorFactory(new UTTLatticeReader::Factory());
    keeper_.takeProcessorFactory(new PsiLatticeReader::Factory());
    keeper_.takeProcessorFactory(new PsiLatticeWriter::Factory());
    keeper_.takeProcessorFactory(new SimpleLatticeWriter::Factory());
    keeper_.takeProcessorFactory(new TpTokenizer::Factory());
    keeper_.takeProcessorFactory(new SrxSegmenter::Factory());
#if HAVE_POSTGRESQL
    keeper_.takeProcessorFactory(new LemmatizerAnnotator<LexDbLemmatizer>::Factory());
#endif
#if HAVE_JAVA
    keeper_.takeProcessorFactory(new LemmatizerAnnotator<Morfologik>::Factory());
#endif
#if HAVE_PUDDLE
      keeper_.takeProcessorFactory(new poleng::bonsai::puddle::Puddle::Factory());
#endif

#if HAVE_PERL_BINDINGS
    keeper_.takeProcessorFactory(new PerlSimpleLatticeWriter::Factory());
#endif
}

ProcessorFactory& MainFactoriesKeeper::getProcessorFactory(std::string processorName) {
    return keeper_.getProcessorFactory(processorName);
}

boost::program_options::options_description MainFactoriesKeeper::getProcessorOptions(std::string processorName) {
    return keeper_.getProcessorFactory(processorName).optionsHandled();
}

std::vector<std::string> MainFactoriesKeeper::getProcessorNames() {
    return keeper_.getProcessorNames();
};

MainFactoriesKeeper& MainFactoriesKeeper::getInstance() {
    static MainFactoriesKeeper instance;
    return instance;
}

