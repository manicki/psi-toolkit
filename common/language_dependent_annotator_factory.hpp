#ifndef LANGUAGE_DEPENDENT_ANNOTATOR_FACTORY
#define LANGUAGE_DEPENDENT_ANNOTATOR_FACTORY

#include <boost/program_options/options_description.hpp>

#include "annotator_factory.hpp"

/*!
  Factories for processing whose behaviour depends on the language of the text
  should be derived from <this class>. This way options such `--lang` are
  provided.
*/

class LanguageDependentAnnotatorFactory : public AnnotatorFactory {

public:
    virtual ~LanguageDependentAnnotatorFactory();

private:

    virtual boost::program_options::options_description doOptionsHandled();

    virtual void doAddLanguageIndependentOptionsHandled(
        boost::program_options::options_description& optionsDescription) = 0;

};

#endif
