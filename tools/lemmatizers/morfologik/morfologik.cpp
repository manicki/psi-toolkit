#include "morfologik.hpp"
#include <iostream>
#include <boost/algorithm/string.hpp>

std::string Morfologik::tagSeparator = "+";

Morfologik::Morfologik() {
	jenv = NULL;

	JavaVirtualMachine *jvm = JavaVirtualMachine::Instance();
	jenv = jvm->getENV();

	initializePolishStemmer();
	initializeList();
	initializeWordData();
	initializeString();
}

std::list<AnnotationItem> Morfologik::stems(std::string & word) {
	
	std::multimap<std::string, std::string> rawStems = stem(word);
	std::multimap<std::string, std::string>::iterator it;
	std::list<AnnotationItem> stems;

	for (it = rawStems.begin(); it != rawStems.end(); it++) {
		std::string stem = it->first;
		std::string tag = it->second;

		std::list<AnnotationItem> tempAnnotations = 
			createAnnotation(stem, tag);
		stems.splice(stems.end(), tempAnnotations);
	}

	return stems;
}

std::list<AnnotationItem> Morfologik::createAnnotation(
	std::string & stem, std::string & tag
) {

	std::list<AnnotationItem> annotations;
	std::vector<std::map<std::string, std::string> > tags = 
		tagsParser.parse(tag);
	std::map<std::string, std::string>::iterator it;

	for (int i = 0; i < (int)tags.size(); i++) {
	
		AnnotationItem annotation(stem);
		for (it = tags[i].begin(); it != tags[i].end(); it++) {
			annotationManager.setValue(annotation, it->first, it->second);
		}
		annotations.push_back(annotation);
	}

	return annotations;
}

std::multimap<std::string, std::string> Morfologik::stem(
	const std::string & word
) {

	jstring jword = jenv->NewStringUTF(word.c_str());	
	jobject objList = (jobject)jenv->CallObjectMethod
		(objPolishStemmer, midPolishStemmerLookup, jword);
	jenv->DeleteLocalRef(jword);

	int stemsCount = (int)jenv->CallIntMethod(objList, midListGetSize, NULL);

	jobject objWordData = NULL;
	jobject objForString = NULL;
	jstring text = NULL;
	const char *pstem = NULL;
	const char *ptags = NULL;
	std::multimap<std::string, std::string> stems;
	
	for (int i = 0; i < stemsCount; i++) {
		objWordData = (jobject)jenv->CallObjectMethod
			(objList, midListGetElement, (jint)i);

		// Get stem of word.
		objForString = jenv->CallObjectMethod
			(objWordData, midWordDataGetStem, NULL);
		text = (jstring)jenv->CallObjectMethod
			(objForString, midStringToString, NULL);
		pstem = jenv->GetStringUTFChars(text, NULL);
		if (pstem == NULL) {
			/* OutOfMemoryError already throw */
		}
		
		// Get tags of word.
		objForString = jenv->CallObjectMethod(
			objWordData, midWordDataGetTag, NULL);
		text = (jstring)jenv->CallObjectMethod
			(objForString, midStringToString, NULL);
		ptags = jenv->GetStringUTFChars(text, NULL);
		if (pstem == NULL) {
			/* OutOfMemoryError already throw */
		}

		// Split tags if it is neccessary.
		size_t shouldBeSplit = std::string(ptags).find(tagSeparator);
		if (shouldBeSplit != std::string::npos) {
			std::vector<std::string> tags;
			boost::split(tags, ptags, boost::is_any_of(tagSeparator));

			for (int j = 0; j < (int)tags.size(); j++) {
				stems.insert(std::pair<std::string, std::string>
					(pstem, tags[j]) 
				);
			}
		}
		else {
			stems.insert(std::pair<std::string, std::string>(pstem, ptags) );
		}
		
		jenv->ReleaseStringUTFChars(text, pstem);
		jenv->ReleaseStringUTFChars(text, ptags);
	}
	
	jenv->DeleteLocalRef(objList);
	jenv->DeleteLocalRef(objWordData);
	jenv->DeleteLocalRef(objForString);
	jenv->DeleteLocalRef(text);
	return stems;
}

void Morfologik::initializePolishStemmer() {
	clsPolishStemmer = NULL;
	objPolishStemmer = NULL;
	midPolishStemmerConstructor = NULL;
	midPolishStemmerLookup = NULL;

	clsPolishStemmer = jenv->FindClass("morfologik/stemming/PolishStemmer");
	if (clsPolishStemmer != NULL) {
		midPolishStemmerConstructor = jenv->GetMethodID
			(clsPolishStemmer, "<init>", "()V");
		midPolishStemmerLookup = jenv->GetMethodID
			(clsPolishStemmer, "lookup", "(Ljava/lang/CharSequence;)Ljava/util/List;");
	}
	else {
		printf("clsPolishStemmer is NULL\n");	// FIXME: should be an exception
	}
	objPolishStemmer = jenv->NewObject
		(clsPolishStemmer, midPolishStemmerConstructor, NULL);
}

void Morfologik::initializeList() {
	clsList = NULL;
	midListGetSize = NULL;
	midListGetElement = NULL;

	clsList = jenv->FindClass("java/util/List");
	if (clsList != NULL) {
		midListGetSize = jenv->GetMethodID(clsList, "size", "()I");
		midListGetElement = jenv->GetMethodID
			(clsList, "get", "(I)Ljava/lang/Object;");
	}
}

void Morfologik::initializeWordData() {
	clsWordData = NULL;
	midWordDataGetStem = NULL;
	midWordDataGetTag = NULL;

	clsWordData = jenv->FindClass("morfologik/stemming/WordData");
	if (clsWordData != NULL) {
		midWordDataGetStem = jenv->GetMethodID
			(clsWordData, "getStem", "()Ljava/lang/CharSequence;");
		midWordDataGetTag = jenv->GetMethodID
			(clsWordData, "getTag", "()Ljava/lang/CharSequence;");
	}
}

void Morfologik::initializeString() {
	clsString = NULL;
	midStringToString = NULL;

	clsString = jenv->FindClass("java/lang/String");
	if (clsString != NULL) {
		midStringToString = jenv->GetMethodID
			(clsString, "toString", "()Ljava/lang/String;");
	}
}

