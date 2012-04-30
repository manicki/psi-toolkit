#include "annotator_promise.hpp"

AnnotatorPromise::AnnotatorPromise(
    AnnotatorFactory* annotatorFactory,
    const boost::program_options::variables_map& options)
    :ProcessorPromise(annotatorFactory, options) {
}

Processor* AnnotatorPromise::doCreateProcessor() {
    return dynamic_cast<AnnotatorFactory*>(factory_)->createAnnotator(options_);
}

std::list<std::list<std::string> > AnnotatorPromise::doRequiredLayerTags() {
    return dynamic_cast<AnnotatorFactory*>(factory_)->requiredLayerTags();
}

std::list<std::list<std::string> > AnnotatorPromise::doOptionalLayerTags() {
    return dynamic_cast<AnnotatorFactory*>(factory_)->optionalLayerTags();
}

std::list<std::string> AnnotatorPromise::doProvidedLayerTags() {
    return dynamic_cast<AnnotatorFactory*>(factory_)->providedLayerTags();
}

AnnotatorFactory::LanguagesHandling AnnotatorPromise::doLanguagesHandling() const {
    return dynamic_cast<AnnotatorFactory*>(factory_)->languagesHandling(options_);
}

std::list<std::string> AnnotatorPromise::doLanguagesHandled() const {
    return dynamic_cast<AnnotatorFactory*>(factory_)->languagesHandled(options_);
}

bool AnnotatorPromise::doIsAnnotator() const {
    return true;
}

double AnnotatorPromise::doGetQualityScore() const {
    return dynamic_cast<AnnotatorFactory*>(factory_)->getQualityScore(options_);
}

double AnnotatorPromise::doGetEstimatedTime() const {
    return dynamic_cast<AnnotatorFactory*>(factory_)->getEstimatedTime(options_);
}

std::string AnnotatorPromise::doGetName() const {
    return dynamic_cast<AnnotatorFactory*>(factory_)->getName();
}

boost::shared_ptr<ProcessorPromise> AnnotatorPromise::doCloneWithLanguageSet(
    const std::string& langCode) {

    if (languagesHandling() != AnnotatorFactory::LANGUAGE_DEPENDENT) {
        return boost::shared_ptr<ProcessorPromise>(
            new AnnotatorPromise(dynamic_cast<AnnotatorFactory*>(factory_), options_));
    }

    boost::program_options::variables_map newOptions = options_;

    boost::program_options::variables_map::iterator it(
        newOptions.find("lang"));
    boost::program_options::variable_value & vx(it->second);
    vx.value() = boost::any(langCode);

    return boost::shared_ptr<ProcessorPromise>(
        new AnnotatorPromise(dynamic_cast<AnnotatorFactory*>(factory_), newOptions));
}
