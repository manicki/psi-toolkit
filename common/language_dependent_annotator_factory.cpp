#include "language_dependent_annotator_factory.hpp"

LanguageDependentAnnotatorFactory::~LanguageDependentAnnotatorFactory() {
}

boost::program_options::options_description LanguageDependentAnnotatorFactory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    addLanguageDependentOptions(optionsDescription);

    doAddLanguageIndependentOptionsHandled(optionsDescription);

    return optionsDescription;

}

void LanguageDependentAnnotatorFactory::addLanguageDependentOptions(
    boost::program_options::options_description& optionsDescription) {

    optionsDescription.add_options()
        ("lang", boost::program_options::value<std::string>()->required(), "language")
        ("force-language", boost::program_options::bool_switch()->default_value(false),
         "force using specified language even if a text was resognised otherwise")
        ;
}
