#include "processor_promise.hpp"

#include <assert.h>

Processor* ProcessorPromise::createProcessor() {
    return doCreateProcessor();
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
    return languagesHandled();
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
    const std::string& langCode) const {
    assert(languagesHandling() == AnnotatorFactory::LANGUAGE_DEPENDENT);

    return doCloneWithLanguageSet(langCode);
}
