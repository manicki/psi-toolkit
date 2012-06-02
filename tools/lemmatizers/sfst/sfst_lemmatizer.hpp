#ifndef SFST_HDR
#define SFST_HDR


#ifndef HAVE_CONFIG_H
#define HAVE_CONFIG_H 0
#endif

#ifndef HAVE_BACKWARD_HASH_MAP
#define HAVE_BACKWARD_HASH_MAP 0
#endif

#ifndef HAVE_HASH_MAP
#define HAVE_HASH_MAP 0
#endif

#ifndef HAVE_EXT_HASH_MAP
#define HAVE_EXT_HASH_MAP 0
#endif

#ifndef HAVE_BACKWARD_HASH_SET
#define HAVE_BACKWARD_HASH_SET 0
#endif

#ifndef HAVE_EXT_HASH_SET
#define HAVE_EXT_HASH_SET 0
#endif

#ifndef HAVE_HASH_SET
#define HAVE_HASH_SET 0
#endif

#ifndef SGIext
#define SGIext 0
#endif

#ifndef SGI__gnu_cxx
#define SGI__gnu_cxx 0
#endif

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/filesystem.hpp>


#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm.hpp>

#include "fst.h"
#include "sfst_tags_parser.hpp"
#include "annotation_item_manager.hpp"
#include "lemmatizer_output_iterator.hpp"
#include "annotator_factory.hpp"

#include <map>
#include <list>
#include <set>

class SfstLemmatizer {
public:

    SfstLemmatizer(const boost::program_options::variables_map& options);


    static boost::program_options::options_description optionsHandled();

    static std::string getName();
    static boost::filesystem::path getFile();

    std::list<std::string> getLayerTags();

    std::string getLanguage() const;

    static AnnotatorFactory::LanguagesHandling languagesHandling(
        const boost::program_options::variables_map& options);

    static std::list<std::string> languagesHandled(
        const boost::program_options::variables_map& options);

    void lemmatize(
        const std::string & word,
        AnnotationItemManager & manager,
        LemmatizerOutputIterator & iterator);

    /**
     * Sets the word processing level, default 0.
     * @param level
     *  Word processing level:
     *  0 - normal mode...
     */
    void setLevel(int);

    /**
     * Sets language and reloads dict
     * @param lang
     * lang name
     */
    void setLanguage(std::string);
    void setAutomaton(std::string);
    void setRawRoots(bool raw);

    /**
     * Stems word using sfstLemmatizer tool storing lexical information.
     * @param word
     *  A string containing word to stem.
     * @return
     *  A multimap object with stem-tags pairs. Each stem may have more than
     *  one tag (in Morfologik tags are separated by plus sign).
     * @see simpleStem()
     */
    std::multimap<std::string, std::vector<std::string> > stem(
        const std::string & word);

    /**
     * Stems word using sfstLemmatizer tool.
     * @param word
     *  A string containing word to stem.
     * @return
     *  A vector of found stems with repetitions.
     * @see stem()
     */
    std::vector<std::string> simpleStem(const std::string & word);

private:
    AnnotationItemManager * annotationManager;
    int level;
    std::string language;
    std::string automaton;
    bool rawRoots;

    void stemsOnLemmaLevel(const std::string &, LemmatizerOutputIterator &);
    void stemsOnLexemeLevel(const std::string &, LemmatizerOutputIterator &);
    void stemsOnFormLevel(const std::string &, LemmatizerOutputIterator &);

    //Ugly taken from morfo...
    AnnotationItem createLexemeAnnotation(
        const std::string & stem, const std::string & tag);

    AnnotationItem createFormAnnotation(
        AnnotationItem & lexemeItem,
        const std::string & word,
        std::vector<std::string> &);


    std::set<std::string> getLemmasFromStems(
        std::multimap<std::string, std::vector<std::string> > stems);

    std::set<std::string> getLexemeTagsFromStems(
        std::multimap<std::string, std::vector<std::string> > & stems,
        const std::string & lemma);

    std::vector<std::vector<std::string> > getFormTagsFromLexem(
        std::multimap<std::string, std::vector<std::string> > & stems,
        const std::string & lemma,
        const std::string & lexem);

    static std::string tagSeparator;

    boost::scoped_ptr<SFST::Transducer> transducer;

    /***
     * Outpusts raw word analysis from sfst transducer.
     * @param word
     * A word that will be analysed.
     * @return
     * String vector containing output of transducer.
     */
    std::vector<std::string> wordToRaw(std::string);

    void cookRaw(std::string &);
    void cookTurkishVerb(std::string &);

    std::string getCookedStem(std::string);
    std::vector<std::string> getCookedTags(std::string);

    void initializeTransducer();

    const static std::string DEFAULT_AUTOMATON_FILE_SPEC;
};

#endif
