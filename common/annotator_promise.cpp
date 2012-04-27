#include "annotator_promise.hpp"

AnnotatorPromise::AnnotatorPromise(
    AnnotatorFactory* annotatorFactory,
    const boost::program_options::variables_map& options)
    :annotatorFactory_(annotatorFactory), options_(options) {
}

Processor* AnnotatorPromise::doCreateProcessor() {
    return annotatorFactory_->createAnnotator(options_);
}

std::list<std::list<std::string> > AnnotatorPromise::doRequiredLayerTags() {
    return annotatorFactory_->requiredLayerTags();
}

std::list<std::list<std::string> > AnnotatorPromise::doOptionalLayerTags() {
    return annotatorFactory_->optionalLayerTags();
}

std::list<std::string> AnnotatorPromise::doProvidedLayerTags() {
    return annotatorFactory_->providedLayerTags();
}

AnnotatorFactory::LanguagesHandling AnnotatorPromise::doLanguagesHandling() const {
    return annotatorFactory_->languagesHandling(options_);
}

std::list<std::string> AnnotatorPromise::doLanguagesHandled() const {
    return annotatorFactory_->languagesHandled(options_);
}

bool AnnotatorPromise::doIsAnnotator() const {
    return true;
}

bool AnnotatorPromise::doIsReader() const {
    return false;
}

bool AnnotatorPromise::doIsWriter() const {
    return false;
}
