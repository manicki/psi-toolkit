#include "non_annotator_promise.hpp"

Processor* NonAnnotatorPromise::doCreateProcessor() {
    return factory_->createProcessor(options_);
}

std::list<std::list<std::string> > NonAnnotatorPromise::doRequiredLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::list<std::string> > NonAnnotatorPromise::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> NonAnnotatorPromise::doProvidedLayerTags() {
    return std::list<std::string>();
}

AnnotatorFactory::LanguagesHandling NonAnnotatorPromise::doLanguagesHandling() const {
    return AnnotatorFactory::LANGUAGE_INDEPENDENT;
}

std::list<std::string> NonAnnotatorPromise::doLanguagesHandled() const {
    return std::list<std::string>();
}

bool NonAnnotatorPromise::doIsAnnotator() const {
    return false;
}

double NonAnnotatorPromise::doGetQualityScore() const {
    return factory_->getQualityScore(options_);
}

double NonAnnotatorPromise::doGetEstimatedTime() const {
    return factory_->getEstimatedTime(options_);
}

std::string NonAnnotatorPromise::doGetName() const {
    return factory_->getName();
}

boost::shared_ptr<ProcessorPromise> NonAnnotatorPromise::doCloneWithLanguageSet(
    const std::string& /*langCode*/) {

    return boost::shared_ptr<ProcessorPromise>(
        new NonAnnotatorPromise(factory_, options_));
}
