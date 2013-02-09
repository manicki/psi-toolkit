#ifndef HFST_LEMMATIZER_HDR
#define HFST_LEMMATIZER_HDR

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/filesystem.hpp>

#include "hfst_tags_parser.hpp"
#include "hfst_adapter_interface.hpp"
#include "plugin_manager.hpp"
#include "annotation_item_manager.hpp"
#include "lemmatizer_output_iterator.hpp"
#include "annotator_factory.hpp"
#include "regexp.hpp"

#include "hfst_lemmatizer_types.hpp"

#include "hfst_english_tags_parser.hpp"
#include "hfst_finnish_tags_parser.hpp"
#include "hfst_french_tags_parser.hpp"
#include "hfst_german_tags_parser.hpp"
#include "hfst_italian_tags_parser.hpp"
#include "hfst_swedish_tags_parser.hpp"

#include "hfst_english_stemmer.hpp"
#include "hfst_finnish_stemmer.hpp"
#include "hfst_french_stemmer.hpp"
#include "hfst_german_stemmer.hpp"
#include "hfst_italian_stemmer.hpp"
#include "hfst_swedish_stemmer.hpp"

#include <map>
#include <vector>

class HfstLemmatizer {

    public:
        HfstLemmatizer(const boost::program_options::variables_map& options);
        ~HfstLemmatizer();

        static std::string getName();
        static boost::filesystem::path getFile();
        std::list<std::string> getLayerTags();
        std::string getLanguage() const;

        static AnnotatorFactory::LanguagesHandling languagesHandling(
                const boost::program_options::variables_map& options);

        static std::list<std::string> languagesHandled(
                const boost::program_options::variables_map& options);

        static bool checkRequirements(
                const boost::program_options::variables_map& options,
                std::ostream & message);

        bool lemmatize(const std::string & word, AnnotationItemManager & manager,
                LemmatizerOutputIterator & iterator);

        static boost::program_options::options_description optionsHandled();

        /**
         * Sets the word processing level, default 0.
         * @param level
         *  Word processing level:
         *  0 - does nothing because word correction and normalization are
         *      inavailable in HfstLemmatizer tool
         *  1 - finds lemma, i.e. only the grammatical class is storing
         *  2 - adds lexems
         *  3 - adds all forms
         */
        void setLevel(int);

        void setLanguage(const std::string &);
            
        void setTransducerFile(const std::string &);

        static const std::string DEFAULT_TRANSDUCER_FILE;

        bool isActive();

    private:
        int level_;
        std::string language_;
        std::string transducerFile_;
        HfstAdapterInterface* hfstAdapter_;
        HfstTagsParser* hfstTagsParser_;
        HfstStemmer* hfstStemmer_;

        AnnotationItemManager * annotationManager_;

        void init(const boost::program_options::variables_map& options);
        void initializeHfstTransducer();
        HfstTagsParser* initializeHfstTagsParser(const boost::program_options::variables_map &);
        HfstStemmer* initializeHfstStemmer(const boost::program_options::variables_map &);

        bool stemsOnLemmaLevel_(const std::string &, LemmatizerOutputIterator &);
        bool stemsOnLexemeLevel_(const std::string &, LemmatizerOutputIterator &);
        bool stemsOnFormLevel_(const std::string &, LemmatizerOutputIterator &);

        AnnotationItem createLexemeAnnotation(
                const std::string & stem, std::map<std::string, std::string> attributes);

        AnnotationItem createFormAnnotation(
                AnnotationItem & lexemeItem,
                const std::string & word,
                std::map<std::string, std::string> &);


        std::set<std::string> getLemmasFromStems(
                HfstStems & stems);

        HfstTags getLexemeTagsFromStems(HfstStems & stems, const std::string & lemma);

        HfstTags getFormTagsFromLexeme(HfstStems &, const std::string &,
                std::map<std::string, std::string>);

        bool attributesMatch(std::map<std::string, std::string> specificAttributes,
                std::map<std::string, std::string> generalAttributes);
};

#endif
