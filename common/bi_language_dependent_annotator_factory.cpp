#include "bi_language_dependent_annotator_factory.hpp"

BiLanguageDependentAnnotatorFactory::~BiLanguageDependentAnnotatorFactory() {
}

boost::program_options::options_description
BiLanguageDependentAnnotatorFactory::doOptionsHandled() {

    boost::program_options::options_description optionsDescription("Allowed options");

    addBiLanguageDependentOptions(optionsDescription);

    doAddBiLanguageIndependentOptionsHandled(optionsDescription);

    return optionsDescription;

}

void BiLanguageDependentAnnotatorFactory::addBiLanguageDependentOptions(
    boost::program_options::options_description& optionsDescription) {

    addLanguageDependentOptions(optionsDescription);
    addOnlyBiLanguageDependentOptions(optionsDescription);
}

void BiLanguageDependentAnnotatorFactory::addOnlyBiLanguageDependentOptions(
    boost::program_options::options_description& optionsDescription) {

    optionsDescription.add_options()
        ("trg-lang", boost::program_options::value<std::string>()->required(), "target language");
}

void BiLanguageDependentAnnotatorFactory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription) {

    addOnlyBiLanguageDependentOptions(optionsDescription);

    doAddBiLanguageIndependentOptionsHandled(optionsDescription);
}
