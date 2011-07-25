#include "main_factories_keeper.hpp"

#include "txt_lattice_reader.hpp"
#include "psi_lattice_writer.hpp"
#include "tp_tokenizer.hpp"

MainFactoriesKeeper::MainFactoriesKeeper() {
    keeper_.takeProcessorFactory(new TxtLatticeReader::Factory());
    keeper_.takeProcessorFactory(new PsiLatticeWriter::Factory());
    keeper_.takeProcessorFactory(new TpTokenizer::Factory());
}

ProcessorFactory& MainFactoriesKeeper::getProcessorFactory(std::string processorName) {
    return keeper_.getProcessorFactory(processorName);
}

MainFactoriesKeeper& MainFactoriesKeeper::getInstance() {
    static MainFactoriesKeeper instance;
    return instance;
}

