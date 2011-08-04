#include "morfologik.hpp"
#include <iostream>

Morfologik::Morfologik() {
	jenv = NULL;

	JavaVirtualMachine *jvm = JavaVirtualMachine::Instance();
	jenv = jvm->getENV();

	initializePolishStemmer();
	initializeList();
	initializeWordData();
	initializeString();
}

std::vector<std::string> Morfologik::stem(const std::string& word) {

	jstring jword = jenv->NewStringUTF(word.c_str());	
	objList = (jobject)jenv->CallObjectMethod(objPolishStemmer, midPolishStemmerLookup, jword);
	int stemsCount = (int)jenv->CallIntMethod(objList, midListGetSize, NULL);

	std::vector<std::string> stems;
	jobject objForString = NULL;
	jstring text = NULL;
	const char *ptext = NULL;

	for (int i = 0; i < stemsCount; i++) {
		objWordData = (jobject)jenv->CallObjectMethod(objList, midListGetElement, (jint)i);

		objForString = jenv->CallObjectMethod(objWordData, midWordDataGetStem, NULL);
		text = (jstring)jenv->CallObjectMethod(objForString, midStringToString, NULL);
		ptext = jenv->GetStringUTFChars(text, NULL);
		if (ptext == NULL) {
			/* OutOfMemoryError already throw */
		}
		stems.insert(stems.end(), ptext);
		jenv->ReleaseStringUTFChars(text, ptext);
		
		objForString = jenv->CallObjectMethod(objWordData, midWordDataGetTag, NULL);
		text = (jstring)jenv->CallObjectMethod(objForString, midStringToString, NULL);
		ptext = jenv->GetStringUTFChars(text, NULL);
		stems.insert(stems.end(), ptext);
		jenv->ReleaseStringUTFChars(text, ptext);
	
	}	
	return stems;
}

void Morfologik::initializePolishStemmer() {
	clsPolishStemmer = NULL;
	objPolishStemmer = NULL;
	midPolishStemmerConstructor = NULL;
	midPolishStemmerLookup = NULL;

	clsPolishStemmer = jenv->FindClass("morfologik/stemming/PolishStemmer");
	if (clsPolishStemmer != NULL) {
		midPolishStemmerConstructor = jenv->GetMethodID(clsPolishStemmer, "<init>", "()V");
		midPolishStemmerLookup = jenv->GetMethodID(clsPolishStemmer, "lookup", "(Ljava/lang/CharSequence;)Ljava/util/List;");
	}
	else {
		printf("clsPolishStemmer is NULL\n");	// FIXME: should be an exception
	}

	objPolishStemmer = jenv->NewObject(clsPolishStemmer, midPolishStemmerConstructor, NULL);
	if (objPolishStemmer == NULL) {
		printf("objPolishStemmer is NULL\n"); // FIXME: should be an exception
	}
}

void Morfologik::initializeList() {
	clsList = NULL;
	objList = NULL;
	midListGetSize = NULL;
	midListGetElement = NULL;

	clsList = jenv->FindClass("java/util/List");
	if (clsList != NULL) {
		midListGetSize = jenv->GetMethodID(clsList, "size", "()I");
		midListGetElement = jenv->GetMethodID(clsList, "get", "(I)Ljava/lang/Object;");
	}
	else {
		printf("clsList is NULL\n");	// FIXME: should be an exception
	}
}

void Morfologik::initializeWordData() {
	clsWordData = NULL;
	objWordData = NULL;
	midWordDataGetStem = NULL;
	midWordDataGetTag = NULL;

	clsWordData = jenv->FindClass("morfologik/stemming/WordData");
	if (clsWordData != NULL) {
		midWordDataGetStem = jenv->GetMethodID(clsWordData, "getStem", "()Ljava/lang/CharSequence;");
		midWordDataGetTag = jenv->GetMethodID(clsWordData, "getTag", "()Ljava/lang/CharSequence;");
	}
	else {
		printf("clsWordData is NULL\n");	// FIXME: should be an exception
	}
}

void Morfologik::initializeString() {
	clsString = NULL;
	midStringToString = NULL;

	clsString = jenv->FindClass("java/lang/String");
	if (clsString != NULL) {
		midStringToString = jenv->GetMethodID(clsString, "toString", "()Ljava/lang/String;");
	}
	else {
		printf("clsString is NULL\n");	// FIXME: should be an exception
	}
}
