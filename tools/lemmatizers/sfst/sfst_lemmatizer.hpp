#ifndef SFST_HDR
#define SFST_HDR

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/filesystem.hpp>

#include "fst.h"
#include "sfst_tags_parser.hpp"
#include "annotation_item_manager.hpp"
#include "lemmatizer_output_iterator.hpp"

#include <map>
#include <list>
#include <set>

class SfstLemmatizer {
	public:

    SfstLemmatizer(const boost::program_options::variables_map& options);
    
    static std::string getName();
    
    std::string getLanguage() const;
    
	void lemmatize(
        const std::string & word,
        AnnotationItemManager & manager,
        LemmatizerOutputIterator & iterator
    );
    
    /**
     * Sets the word processing level, default 0.
     * @param level
     *  Word processing level:
     *  0 - normal mode...
     */
    void setLevel(int);
    
    
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
		
		void stemsOnLemmaLevel(const std::string &, LemmatizerOutputIterator &);
		void stemsOnLexemeLevel(const std::string &, LemmatizerOutputIterator &);
		void stemsOnFormLevel(const std::string &, LemmatizerOutputIterator &);

		AnnotationItem createLexemeAnnotation(
			const std::string & stem, std::string & tag
		);
		AnnotationItem createFormAnnotation(
			AnnotationItem & lexemeItem,
			const std::string& word,
			std::map<std::string, std::string> &
		);

		
		std::set<std::string> getLemmasFromStems(
			std::multimap<std::string, std::vector<std::string> > stems
		);
		std::vector<std::string> getLexemeTagsFromStems(
			std::multimap<std::string, std::vector<std::string> > & stems,
			const std::string & lemma
		);
		static std::string tagSeparator;
		
		SFST::Transducer turkishTransducer;
		
		void initializeTurkishTransducer();
		
		SfstTagsParser tagsParser;
};

#endif
