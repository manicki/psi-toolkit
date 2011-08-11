#ifndef MORFOLOGIK_HDR
#define MORFOLOGIK_HDR

#include "java_virtual_machine.hpp"
#include "annotation_item_manager.hpp"
#include "morfologik_tags_parser.hpp"
#include <map>

class Morfologik {
	
public:
	Morfologik();

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
};

#endif

