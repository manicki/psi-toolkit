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

    static void addLanguageDependentOptions(
        boost::program_options::options_description& optionsDescription);

    static LanguagesHandling checkLangOption(
        const boost::program_options::variables_map& options);

private:

    virtual boost::program_options::options_description doOptionsHandled();

    virtual LanguagesHandling doLanguagesHandling(
        const boost::program_options::variables_map& options) const;

    virtual std::list<std::string> doLanguagesHandled(
        const boost::program_options::variables_map& options) const;

    virtual void doAddLanguageIndependentOptionsHandled(
        boost::program_options::options_description& optionsDescription) = 0;

    virtual std::list<std::string> doAllLanguagesHandled() const;

    const static std::string GUESS_VALUE_FOR_LANG_OPTION;
};

#endif
