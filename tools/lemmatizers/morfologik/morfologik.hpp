#ifndef MORFOLOGIK_HDR
#define MORFOLOGIK_HDR

#include "java_virtual_machine.hpp"
#include "morfologik_tags_parser.hpp"
#include "annotation_item_manager.hpp"
#include "lemmatizer_output_iterator.hpp"

#include <map>
#include <list>
#include <set>

class Morfologik {
	
public:

	Morfologik();
	std::string getName();
	std::list<std::string> getLayerTags();

	void lemmatize(
		const std::string & word, 
		AnnotationItemManager & manager, 
		LemmatizerOutputIterator & iterator
	);

	/**
	 * Sets the word processing level, default 0.
	 * @param level
	 * 	Word processing level:
	 * 	0 - does nothing because word correction and normalization are 
	 * 		inavailable in Morfologik tool
	 * 	1 - finds lemma, i.e. only the grammatical class is storing
	 * 	2 - adds lexems
	 * 	3 - adds all forms
	 */
	void setLevel(int);

	friend class MorfologikTests; 

protected:

	/**
	 * Stems word using Morfologik tool storing lexical information.
	 * @param word
	 * 	A string containing word to stem.
	 * @return
	 * 	A multimap object with stem-tags pairs. Each stem may have more than 
	 * 	one tag (in Morfologik tags are separated by plus sign).
	 * @see simpleStem()
	 */
	std::multimap<std::string, std::vector<std::string> > stem(
		const std::string & word);

	/**
	 * Stems word using Morfologik tool without lexical information.
	 * @param word
	 * 	A string containing word to stem.
	 * @return
	 * 	A vector of found stems with repetitions.
	 * @see stem()
	 */
	std::vector<std::string> simpleStem(const std::string & word);

private:

	int level;
	AnnotationItemManager * annotationManager;
//	LemmatizerOutputIterator * outputIterator;

	void stemsOnLemmaLevel(const std::string &, LemmatizerOutputIterator &);
	void stemsOnLexemeLevel(const std::string &, LemmatizerOutputIterator &);
	void stemsOnFormLevel(const std::string &, LemmatizerOutputIterator &);

	AnnotationItem createLexemeAnnotation(
		const std::string & stem, std::string & tag
	);
	AnnotationItem createFormAnnotation(
		AnnotationItem & lexemeItem, std::map<std::string, std::string> &
	);

	std::set<std::string> getLemmasFromStems(
		std::multimap<std::string, std::vector<std::string> > stems
	);
	std::vector<std::string> getLexemeTagsFromStems(
		std::multimap<std::string, std::vector<std::string> > & stems, 
		const std::string & lemma
	);

	JNIEnv * jenv;
	static std::string tagSeparator;

	jclass clsPolishStemmer;
	jobject objPolishStemmer;
	jmethodID midPolishStemmerConstructor;
	jmethodID midPolishStemmerLookup;
	void initializePolishStemmer();

	jclass clsList;
	jmethodID midListGetElement;
	jmethodID midListGetSize;
	void initializeList();

	jclass clsWordData;
	jmethodID midWordDataGetStem;
	jmethodID midWordDataGetTag;
	void initializeWordData();

	jclass clsString;
	jmethodID midStringToString;
	void initializeString();

	const char * getStemByJNI(jobject);
	const char * getTagsByJNI(jobject);

	MorfologikTagsParser tagsParser;
/*
	std::list<AnnotationItem> createAnnotation(
		std::string & stem, std::string & tag
	);
*/
};

#endif

