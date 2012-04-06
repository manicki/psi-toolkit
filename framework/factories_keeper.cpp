#include "factories_keeper.hpp"

#include "logging.hpp"
#include <boost/foreach.hpp>

void FactoriesKeeper::takeProcessorFactory(ProcessorFactory* processorFactory) {
    DEBUG("registering processor " << processorFactory->getName());

    checkAnnotator_(processorFactory);

    BOOST_FOREACH(const std::string& alias, processorFactory->getAliases())
        aliaser_.addAlias(alias, processorFactory->getName());

    nameToFactoryMap_[processorFactory->getName()]
        = boost::shared_ptr<ProcessorFactory>(processorFactory);
}

void FactoriesKeeper::addTagBasedAlias(const std::string& tag, const std::string& alias) {
    aliaser_.addAlias(alias, getBaseAliasForTag(tag));
    aliaser_.addVoidAlias(getBaseAliasForTag(tag));
}

void FactoriesKeeper::addTagBasedIzeAliases(const std::string& tag, const std::string& aliasRoot) {
    addTagBasedAlias(tag, aliasRoot + "ise");
    addTagBasedAlias(tag, aliasRoot + "ize");
    addTagBasedAlias(tag, aliasRoot + "iser");
    addTagBasedAlias(tag, aliasRoot + "izer");
}

void FactoriesKeeper::addAlias(const std::string& alias, const std::string& destination) {
    aliaser_.addAlias(alias, destination);
}

std::string FactoriesKeeper::getBaseAliasForTag(const std::string& tag) {
    return tag + "-generator";
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

std::set<std::string> FactoriesKeeper::getAliasNames() {
    return aliaser_.getAliasNames();
}

std::list<ProcessorFactory*>
FactoriesKeeper::getProcessorFactoriesForName(std::string name) {
    if (nameToFactoryMap_.count(name)) {
        std::list<ProcessorFactory*> returnedList;
        returnedList.push_back(&*nameToFactoryMap_[name]);
        return returnedList;
    }

    if (aliaser_.isAlias(name)) {
        std::list<ProcessorFactory*> returnedList;

        BOOST_FOREACH(std::string destination, aliaser_.getAllDestinations(name)) {
            INFO(name << " is alias for " << destination);
            returnedList.push_back(&getProcessorFactory(destination));
        }

        return returnedList;
    }

    throw UnknownProcessorException(name);
}

void FactoriesKeeper::checkAnnotator_(ProcessorFactory* processorFactory) {
    AnnotatorFactory* annotatorFactory = dynamic_cast<AnnotatorFactory*>(processorFactory);

    if (annotatorFactory) {
        BOOST_FOREACH(std::string tag, annotatorFactory->providedLayerTags()) {
            std::string baseAlias = getBaseAliasForTag(tag);

            if (aliaser_.isAlias(baseAlias)
                // e.g. aspell requires and provides tokens
                // - we don't want to treat it as a tokenizer
                && !isTagRequiredByAnnotator_(tag, annotatorFactory))
                aliaser_.addAlias(baseAlias, annotatorFactory->getName());
        }
    }
}

typedef std::list<std::string> list_of_strings;

bool FactoriesKeeper::isTagRequiredByAnnotator_(
    const std::string& tag, AnnotatorFactory* annotatorFactory) {
    BOOST_FOREACH(const list_of_strings& seq, annotatorFactory->requiredLayerTags())
        BOOST_FOREACH(const std::string& requiredTag, seq)
            if (requiredTag == tag)
                return true;
    return false;
}
