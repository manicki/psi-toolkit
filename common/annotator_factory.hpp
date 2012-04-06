#ifndef ANNOTATOR_FACTORY_HDR
#define ANNOTATOR_FACTORY_HDR

#include <boost/program_options/options_description.hpp>

#include "processor_factory.hpp"
#include "annotator.hpp"

#include <string>
#include <list>

class AnnotatorFactory : public ProcessorFactory {

public:
    Annotator* createAnnotator(const boost::program_options::variables_map& options);

    /**
     * Returns layer tags that must be provided in the lattice for
     * the annotator to proceed.
     *
     * This information can be used by the framework to automagically
     * run other annotators before the given annotator is launched.
     */
    std::list<std::list<std::string> > requiredLayerTags();

    /**
     * Returns layer tags that might be useful for the annotator.
     *
     * This information can be used by the framework to run other
     * (non-obligatory) annotators before the given annotator is launched.
     */
    std::list<std::list<std::string> > optionalLayerTags();

    /**
     * Layer tags provided by the given annotator. This information
     * could be used by the framework when considering `requiredLayerTags`
     * and `optionalLayerTags` of another annotator.
     */
    std::list<std::string> providedLayerTags();

    enum LanguagesHandling {
        /**
         * The language of annotated edges is irrelevant for the annotator.
         */
        LANGUAGE_INDEPENDENT,

        /**
         * The annotator is inherently limited to just one language
         * (not because a rule set for only one language happened to
         * be specified!).
         */
        JUST_ONE_LANGUAGE,

        /**
         * The annotator can handle many languages and its working
         * depends on the language (e.g. different rule sets or lexica
         * have to selected for each language handled).
         */
        LANGUAGE_DEPENDENT,

        /**
         * The annotator is a language guesser.
         */
        LANGUAGE_GUESSER
    };

    /**
     * Whether annotator depends on the language or not.
     */
    LanguagesHandling languagesHandling(
        const boost::program_options::variables_map& options) const;

    /**
     * Returns languages handled by the annotator.
     *
     * Not that for language-dependent annotators (i.e. if
     * languagesHandling(...) == LANGUAGE_DEPENDENT) a one-element
     * list (e.g. if a rule set for just one language has been
     * specified for the time being) or even an empty list (if the
     * annotator is under construction) might be returned.
     *
     * Empty list should be returned if languagesHandling(...) ==
     * LANGUAGE_INDEPENDENT
     */
    std::list<std::string> languagesHandled(
        const boost::program_options::variables_map& options) const;

private:
    virtual Annotator* doCreateProcessor(const boost::program_options::variables_map& options);

    virtual Annotator* doCreateAnnotator(const boost::program_options::variables_map& options) = 0;


    virtual std::list<std::list<std::string> > doRequiredLayerTags() = 0;

    virtual std::list<std::list<std::string> > doOptionalLayerTags() = 0;

    virtual std::list<std::string> doProvidedLayerTags() = 0;

    virtual std::string doGetContinuation(
        const boost::program_options::variables_map& options) const;

    virtual LanguagesHandling doLanguagesHandling(
        const boost::program_options::variables_map& options) const = 0;

    virtual std::list<std::string> doLanguagesHandled(
        const boost::program_options::variables_map& options) const = 0;


};

#endif
