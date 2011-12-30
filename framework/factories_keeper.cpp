#include "factories_keeper.hpp"

#include "logging.hpp"

void FactoriesKeeper::takeProcessorFactory(ProcessorFactory* processorFactory) {
    DEBUG("registering processor " << processorFactory->getName());

    nameToFactoryMap_[processorFactory->getName()]
        = boost::shared_ptr<ProcessorFactory>(processorFactory);
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

std::vector<std::string> FactoriesKeeper::getProcessorNames() {
    std::vector<std::string> names;

    std::map<std::string, boost::shared_ptr<ProcessorFactory> >::iterator it;
    for (it = nameToFactoryMap_.begin(); it != nameToFactoryMap_.end(); ++it) {
        names.push_back(it->first);
    }

    return names;
}
