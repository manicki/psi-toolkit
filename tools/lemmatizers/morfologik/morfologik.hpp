#ifndef MORFOLOGIK_HDR
#define MORFOLOGIK_HDR

#include "java_virtual_machine.hpp"
#include <vector>

class Morfologik {
	
	public:
		Morfologik(); 
		std::vector<std::string> stem(const std::string& word);

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

