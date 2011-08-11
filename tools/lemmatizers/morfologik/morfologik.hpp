#ifndef MORFOLOGIK_HDR
#define MORFOLOGIK_HDR

#include <map>
#include <list>

#include "java_virtual_machine.hpp"
#include "morfologik_tags_parser.hpp"
#include "annotation_item_manager.hpp"

class Morfologik {
	
public:
	Morfologik();
	std::list<AnnotationItem> stems(std::string & word);

	friend class MorfologikTests; 

protected:
	std::multimap<std::string, std::string> stem(const std::string & word);

private:
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

	MorfologikTagsParser tagsParser;

	AnnotationItemManager annotationManager;
	std::list<AnnotationItem> createAnnotation(
		std::string & stem, std::string & tag
	);
};

#endif

