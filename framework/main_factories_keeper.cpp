#include "main_factories_keeper.hpp"

#include "txt_lattice_reader.hpp"

MainFactoriesKeeper::MainFactoriesKeeper() {
    keeper_.takeProcessorFactory(new TxtLatticeReader::Factory());
}

ProcessorFactory& MainFactoriesKeeper::getProcessorFactory(std::string processorName) {
    return keeper_.getProcessorFactory(processorName);
}

MainFactoriesKeeper& MainFactoriesKeeper::getInstance() {
    static MainFactoriesKeeper instance;
    return instance;
}

