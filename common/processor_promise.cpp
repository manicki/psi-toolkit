#include "processor_promise.hpp"

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
