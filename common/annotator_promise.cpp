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

double AnnotatorPromise::doGetQualityScore() const {
    return annotatorFactory_->getQualityScore(options_);
}

double AnnotatorPromise::doGetEstimatedTime() const {
    return annotatorFactory_->getEstimatedTime(options_);
}

std::string AnnotatorPromise::doGetName() const {
    return annotatorFactory_->getName();
}

boost::shared_ptr<ProcessorPromise> AnnotatorPromise::doCloneWithLanguageSet(
    const std::string& langCode) const {

    boost::program_options::variables_map newOptions = options_;

    boost::program_options::variables_map::iterator it(
        newOptions.find("lang"));
    boost::program_options::variable_value & vx(it->second);
    vx.value() = boost::any(langCode);

    return boost::shared_ptr<ProcessorPromise>(
        new AnnotatorPromise(annotatorFactory_, newOptions));
}
