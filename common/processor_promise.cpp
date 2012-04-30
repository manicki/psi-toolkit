#include "processor_promise.hpp"

#include <assert.h>

bool ProcessorPromise::checkRequirements(std::ostream & message) const {
    return factory_->checkRequirements(options_, message);
}

std::string ProcessorPromise::getContinuation() const {
    return factory_->getContinuation(options_);
}

boost::shared_ptr<Processor> ProcessorPromise::createProcessor() {
    if (!processor_)
        processor_.reset(doCreateProcessor());

    return processor_;
}

std::list<std::list<std::string> > ProcessorPromise::requiredLayerTags() {
    return doRequiredLayerTags();
}

std::list<std::list<std::string> > ProcessorPromise::optionalLayerTags() {
    return doOptionalLayerTags();
}

std::list<std::string> ProcessorPromise::providedLayerTags() {
    return doProvidedLayerTags();
}

AnnotatorFactory::LanguagesHandling ProcessorPromise::languagesHandling() const {
    return doLanguagesHandling();
}

std::list<std::string> ProcessorPromise::languagesHandled() const {
    return doLanguagesHandled();
}

bool ProcessorPromise::isAnnotator() const {
    return doIsAnnotator();
}

double ProcessorPromise::getQualityScore() const {
    return doGetQualityScore();
}

double ProcessorPromise::getEstimatedTime() const {
    return doGetEstimatedTime();
}

std::string ProcessorPromise::getName() const {
    return doGetName();
}

boost::shared_ptr<ProcessorPromise> ProcessorPromise::cloneWithLanguageSet(
    const std::string& langCode) {

    return doCloneWithLanguageSet(langCode);
}
