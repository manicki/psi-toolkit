#include "factories_keeper.hpp"

#include "logging.hpp"

void FactoriesKeeper::takeProcessorFactory(ProcessorFactory* processorFactory) {
    DEBUG("registering processor " << processorFactory->getName());

    nameToFactoryMap_[processorFactory->getName()] = boost::shared_ptr<ProcessorFactory>(processorFactory);
}


ProcessorFactory& FactoriesKeeper::getProcessorFactory(std::string processorName) {
    return *nameToFactoryMap_[processorName];
}



