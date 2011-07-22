#include "factories_keeper.hpp"

void FactoriesKeeper::takeProcessorFactory(ProcessorFactory* processorFactory) {
    nameToFactoryMap_[processorFactory->getName()] = boost::shared_ptr<ProcessorFactory>(processorFactory);
}


ProcessorFactory& FactoriesKeeper::getProcessorFactory(std::string processorName) {
    return *nameToFactoryMap_[processorName];
}



