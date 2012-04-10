#ifndef MORFOLOGIK_HDR
#define MORFOLOGIK_HDR

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/filesystem.hpp>

#include "java_virtual_machine.hpp"
#include "morfologik_tags_parser.hpp"
#include "annotation_item_manager.hpp"
#include "lemmatizer_output_iterator.hpp"
#include "annotator_factory.hpp"

#include <map>
#include <list>
#include <set>

class Morfologik {

public:

    Morfologik(const boost::program_options::variables_map& options);

    static std::string getName();
    static boost::filesystem::path getFile();
    std::list<std::string> getLayerTags();
    std::string getLanguage() const;

    static AnnotatorFactory::LanguagesHandling languagesHandling(
        const boost::program_options::variables_map& options);

    static std::list<std::string> languagesHandled(
        const boost::program_options::variables_map& options);

    void lemmatize(const std::string & word, AnnotationItemManager & manager,
        LemmatizerOutputIterator & iterator);

    static boost::program_options::options_description optionsHandled();

    /**
     * Sets the word processing level, default 0.
     * @param level
     *  Word processing level:
     *  0 - does nothing because word correction and normalization are
     *      inavailable in Morfologik tool
     *  1 - finds lemma, i.e. only the grammatical class is storing
     *  2 - adds lexems
     *  3 - adds all forms
     */
    void setLevel(int);

    void setDictionary(const std::string &);

    /**
     * Stems word using Morfologik tool storing lexical information.
     * @param word
     *  A string containing word to stem.
     * @return
     *  A multimap object with stem-tags pairs. Each stem may have more than
     *  one tag (in Morfologik tags are separated by plus sign).
     * @see simpleStem()
     */
    std::multimap<std::string, std::vector<std::string> > stem(const std::string & word);

    /**
     * Stems word using Morfologik tool without lexical information.
     * @param word
     *  A string containing word to stem.
     * @return
     *  A vector of found stems with repetitions.
     * @see stem()
     */
    std::vector<std::string> simpleStem(const std::string & word);

private:

    static const std::string TAG_SEPARATORS;
    static const std::vector<std::string> DICTIONARIES;

    int level_;
    std::string dictionary_;
    MorfologikTagsParser tagsParser_;

    AnnotationItemManager * annotationManager_;

    void stemsOnLemmaLevel_(const std::string &, LemmatizerOutputIterator &);
    void stemsOnLexemeLevel_(const std::string &, LemmatizerOutputIterator &);
    void stemsOnFormLevel_(const std::string &, LemmatizerOutputIterator &);

    AnnotationItem createLexemeAnnotation_(const std::string& stem, std::string& tag);
    AnnotationItem createFormAnnotation_(AnnotationItem& lexemeItem, const std::string& word,
        std::map<std::string, std::string>&);

    std::set<std::string> getLemmasFromStems_(
        std::multimap<std::string, std::vector<std::string> > stems);
    std::vector<std::string> getLexemeTagsFromStems_(
        std::multimap<std::string, std::vector<std::string> >& stems, const std::string& lemma);

    JNIEnv * jenv_;

    jclass clsPolishStemmer_;
    jobject objPolishStemmer_;
    jmethodID midPolishStemmerConstructor_;
    jmethodID midPolishStemmerLookup_;
    void initializePolishStemmer_();

    jclass clsList_;
    jmethodID midListGetElement_;
    jmethodID midListGetSize_;
    void initializeList_();

    jclass clsWordData_;
    jmethodID midWordDataGetStem_;
    jmethodID midWordDataGetTag_;
    void initializeWordData_();

    jclass clsString_;
    jmethodID midStringToString_;
    void initializeString_();

    jclass enmDictionary_;
    jfieldID fidDictionary_;
    jobject objDictionary_;
    void initializeDictionary_();

    const char * getStemByJNI_(jobject);
    const char * getTagsByJNI_(jobject);
};

#endif
