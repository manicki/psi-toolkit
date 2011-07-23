#include "annotator_factory.hpp"

Annotator* AnnotatorFactory::createAnnotator(
    boost::program_options::variables_map options) {
    return dynamic_cast<Annotator*>(createProcessor(options));
}

Annotator* AnnotatorFactory::doCreateProcessor(
    boost::program_options::variables_map options) {
    return doCreateAnnotator(options);
}




