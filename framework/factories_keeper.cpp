#include "factories_keeper.hpp"

#include "logging.hpp"

void FactoriesKeeper::takeProcessorFactory(ProcessorFactory* processorFactory) {
    DEBUG("registering processor " << processorFactory->getName());

    nameToFactoryMap_[processorFactory->getName()] = boost::shared_ptr<ProcessorFactory>(processorFactory);
}

ProcessorFactory& FactoriesKeeper::getProcessorFactory(std::string processorName) {
    if (nameToFactoryMap_.count(processorName))
        return *nameToFactoryMap_[processorName];

    throw UnknownProcessorException(processorName);
}

FactoriesKeeper::UnknownProcessorException::UnknownProcessorException(
    const std::string& processorName)
    :Exception(std::string("unknown processor `") + processorName + "`") {
}


