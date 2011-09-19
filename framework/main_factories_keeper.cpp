#include "main_factories_keeper.hpp"

#include "config.h"

#include "txt_lattice_reader.hpp"
#include "utt_lattice_reader.hpp"
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

MainFactoriesKeeper::MainFactoriesKeeper() {
    keeper_.takeProcessorFactory(new TxtLatticeReader::Factory());
    keeper_.takeProcessorFactory(new UTTLatticeReader::Factory());
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
}

ProcessorFactory& MainFactoriesKeeper::getProcessorFactory(std::string processorName) {
    return keeper_.getProcessorFactory(processorName);
}

MainFactoriesKeeper& MainFactoriesKeeper::getInstance() {
    static MainFactoriesKeeper instance;
    return instance;
}

