#ifndef BI_LANGUAGE_DEPENDENT_ANNOTATOR_FACTORY
#define BI_LANGUAGE_DEPENDENT_ANNOTATOR_FACTORY

#include <boost/program_options/options_description.hpp>

#include "language_dependent_annotator_factory.hpp"


class BiLanguageDependentAnnotatorFactory : public LanguageDependentAnnotatorFactory {

public:
    virtual ~BiLanguageDependentAnnotatorFactory();

    static void addBiLanguageDependentOptions(
        boost::program_options::options_description& optionsDescription);

    static void addOnlyBiLanguageDependentOptions(
        boost::program_options::options_description& optionsDescription);

private:

    virtual boost::program_options::options_description doOptionsHandled();

    virtual void doAddBiLanguageIndependentOptionsHandled(
        boost::program_options::options_description& optionsDescription) = 0;

    virtual void doAddLanguageIndependentOptionsHandled(
        boost::program_options::options_description& optionsDescription);

};

#endif
