#include "morfologik.hpp"
#include "logging.hpp"
#include "config.hpp"

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/assign.hpp>

std::string Morfologik::tagSeparator = "+";

const std::vector<std::string> Morfologik::DICTIONARIES = boost::assign::list_of
    ("MORFOLOGIK")/*("MORFEUSZ")*/("COMBINED");

Morfologik::Morfologik(const boost::program_options::variables_map& options)
    : annotationManager(NULL), level(3), dictionary_("MORFOLOGIK")
{
    if (options.count("level") > 0) setLevel(options["level"].as<int>());
    if (options.count("dict") > 0) setDictionary(options["dict"].as<std::string>());

    jenv = NULL;
    JavaVirtualMachine *jvm = JavaVirtualMachine::Instance();
    jenv = jvm->getENV();

    initializeDictionary();
    initializePolishStemmer();
    initializeList();
    initializeWordData();
    initializeString();
}

std::string Morfologik::getName() {
    return "morfologik";
}

boost::filesystem::path Morfologik::getFile() {
    return __FILE__;
}

std::list<std::string> Morfologik::getLayerTags() {
    std::list<std::string> layerTags;
    layerTags.push_back("morfologik");
    layerTags.push_back("morfologik-tagset");

    return layerTags;
}

std::string Morfologik::getLanguage() const {
    return "pl";
}

void Morfologik::setLevel(int lvl) {
    if (0 <= lvl && lvl <= 3) {
        level = lvl;
    }
}

void Morfologik::setDictionary(const std::string& dict) {
    if (std::find(DICTIONARIES.begin(), DICTIONARIES.end(), dict) != DICTIONARIES.end()) {
        dictionary_ = dict;
    }
    else {
        ERROR("An unknown dictionary " << dict << ", thereby dictionary is still set to "
            << dictionary_);
    }
}

void Morfologik::lemmatize(const std::string & word,
    AnnotationItemManager & manager, LemmatizerOutputIterator & iterator
) {
    annotationManager = &manager;

    switch (level) {
        case 0:
            break;
        case 1:
            stemsOnLemmaLevel(word, iterator);
            break;
        case 2:
            stemsOnLexemeLevel(word, iterator);
            break;
        default:
            stemsOnFormLevel(word, iterator);
    }
}

boost::program_options::options_description Morfologik::optionsHandled() {
    boost::program_options::options_description desc("Allowed options");

    std::string dictionaryDescription = "set dictionary, one of "
        + boost::algorithm::join(DICTIONARIES, ", ");

    desc.add_options()
        ("level", boost::program_options::value<int>()->default_value(3),
            "set word processing level 0-3")
        ("dict", boost::program_options::value<std::string>()
            ->default_value(DICTIONARIES[0]), dictionaryDescription.c_str())
    ;

    return desc;
}

void Morfologik::stemsOnLemmaLevel(
    const std::string & word, LemmatizerOutputIterator & outputIterator
) {

    std::vector<std::string> stems = simpleStem(word);
    std::vector<std::string>::iterator i;

    DEBUG("found stems of word [" << word << "] on lemma level: ["
        << boost::algorithm::join(stems, ", ") << "]");

    // Remove duplicated values
    i = unique(stems.begin(), stems.end());
    stems.resize(i - stems.begin() );

    for (i = stems.begin(); i != stems.end(); ++i) {
        std::string stem = *i;
        outputIterator.addLemma(stem);
    }
}

void Morfologik::stemsOnLexemeLevel(
    const std::string & word, LemmatizerOutputIterator & outputIterator
) {

    std::multimap<std::string, std::vector<std::string> > stems = stem(word);

    std::set<std::string> lemmas = getLemmasFromStems(stems);
    std::set<std::string>::iterator lem;

    DEBUG("found stems of word [" << word << "] on lexeme level: ["
        << boost::algorithm::join(lemmas, ", ") << "]");

    for (lem = lemmas.begin(); lem != lemmas.end(); ++lem) {
        outputIterator.addLemma(*lem);

        std::vector<std::string> lexemeTags =
            getLexemeTagsFromStems(stems, *lem);
        std::vector<std::string>::iterator lxt;

        for (lxt = lexemeTags.begin(); lxt != lexemeTags.end(); ++lxt) {
            AnnotationItem lexItem = createLexemeAnnotation(*lem, *lxt);
            outputIterator.addLexeme(lexItem);
        }
    }
}

std::set<std::string> Morfologik::getLemmasFromStems(
    std::multimap<std::string, std::vector<std::string> > stems
) {
    std::set<std::string> lemmas;

    std::multimap<std::string, std::vector<std::string> >::iterator s;
    for (s = stems.begin(); s != stems.end(); ++s) {
        lemmas.insert(lemmas.end(), s->first);
    }

    return lemmas;
}

std::vector<std::string> Morfologik::getLexemeTagsFromStems(
    std::multimap<std::string, std::vector<std::string> > & stems,
    const std::string & lemma
) {
    std::vector<std::string> tags;
    std::vector<std::string>::iterator t;

    std::multimap<std::string, std::vector<std::string> >::iterator s;
    for (s = stems.begin(); s != stems.end(); ++s) {

        if (s->first == lemma) {
            t = (s->second).begin();
            tags.insert(tags.begin(), *t);
        }
    }

    return tags;
}

AnnotationItem Morfologik::createLexemeAnnotation(const std::string& stem, std::string& tag) {

    std::map<std::string, std::string> attributes = tagsParser.getLexemeAttributes(tag);
    std::map<std::string, std::string>::iterator atr;

    std::string partOfSpeech = attributes["pos"];
    std::string wordId = stem + LEMMA_CATEGORY_SEPARATOR + partOfSpeech;
    attributes.erase("pos");

    AnnotationItem lexeme(partOfSpeech, StringFrag(wordId));

    for (atr = attributes.begin(); atr != attributes.end(); ++atr) {
        annotationManager->setValue(lexeme, atr->first, atr->second);
    }

    return lexeme;
}

void Morfologik::stemsOnFormLevel(const std::string & word,
    LemmatizerOutputIterator & outputIterator) {

    std::multimap<std::string, std::vector<std::string> > stems = stem(word);

    std::set<std::string> lemmas = getLemmasFromStems(stems);
    std::set<std::string>::iterator lem;

    DEBUG("found stems of word [" << word << "] on form level: ["
        << boost::algorithm::join(lemmas, ", ") << "]");

    for (lem = lemmas.begin(); lem != lemmas.end(); ++lem) {
        outputIterator.addLemma(*lem);

        std::multimap<std::string, std::vector<std::string> >::iterator lex;

        for (lex = stems.equal_range(*lem).first;
            lex != stems.equal_range(*lem).second;
            ++lex) {

            std::vector<std::string> tags = lex->second;
            std::vector<std::string>::iterator tag = tags.begin();

            AnnotationItem lexItm = createLexemeAnnotation(*lem, *tag);
            outputIterator.addLexeme(lexItm);

            for (tag = tags.begin(); tag != tags.end(); ++tag) {

                std::vector<std::map<std::string, std::string> > forms =
                    tagsParser.getFormAttributes(*tag);
                std::vector<std::map<std::string, std::string> >::iterator frm;

                for (frm = forms.begin(); frm != forms.end(); ++frm) {
                    AnnotationItem frmItm = createFormAnnotation(lexItm, word, *frm);
                    outputIterator.addForm(frmItm);
                }
            }

        }
    }
}

AnnotationItem Morfologik::createFormAnnotation(AnnotationItem & lexemeItem,
    const std::string& word, std::map<std::string, std::string> & attributes) {

    AnnotationItem formItem(lexemeItem, word);

    std::map<std::string, std::string>::iterator atr;
    for (atr = attributes.begin(); atr != attributes.end(); ++atr) {
        annotationManager->setValue(formItem, atr->first, atr->second);
    }
    return formItem;
}

std::vector<std::string> Morfologik::simpleStem(const std::string & word) {
    jstring jword = jenv->NewStringUTF(word.c_str());
    jobject objList = (jobject)jenv->CallObjectMethod
        (objPolishStemmer, midPolishStemmerLookup, jword);
    jenv->DeleteLocalRef(jword);

    int stemsCount = (int)jenv->CallIntMethod(objList, midListGetSize, NULL);
    jobject objWordData = NULL;
    const char *pstem = NULL;
    std::vector<std::string> result;

    for (int i = 0; i < stemsCount; ++i) {
        objWordData = (jobject)jenv->CallObjectMethod(objList, midListGetElement, (jint)i);
        pstem = getStemByJNI(objWordData);
        result.insert(result.begin(), pstem);
    }

    jenv->DeleteLocalRef(objList);
    jenv->DeleteLocalRef(objWordData);

    return result;
}

std::multimap<std::string, std::vector<std::string> > Morfologik::stem(
    const std::string & word) {

    jstring jword = jenv->NewStringUTF(word.c_str());
    jobject objList = (jobject)jenv->CallObjectMethod
        (objPolishStemmer, midPolishStemmerLookup, jword);
    jenv->DeleteLocalRef(jword);

    int stemsCount = (int)jenv->CallIntMethod(objList, midListGetSize, NULL);
    DEBUG("Found " << stemsCount << " stems for word " << word);

    jobject objWordData = NULL;
    const char *pstem = NULL;
    const char *ptags = NULL;
    std::multimap<std::string, std::vector<std::string> > result;
    std::vector<std::string> tags;

    for (int i = 0; i < stemsCount; ++i) {
        objWordData = (jobject)jenv->CallObjectMethod(objList, midListGetElement, (jint)i);

        pstem = getStemByJNI(objWordData);
        ptags = getTagsByJNI(objWordData);

        tags.clear();
        boost::split(tags, ptags, boost::is_any_of(tagSeparator));
        result.insert(std::pair<std::string, std::vector<std::string> > (pstem, tags));
    }

    jenv->DeleteLocalRef(objList);
    jenv->DeleteLocalRef(objWordData);
    return result;
}

const char* Morfologik::getStemByJNI(jobject objWordData) {
    const char *pstem = NULL;

    jobject objForString = jenv->CallObjectMethod(objWordData, midWordDataGetStem, NULL);
    jstring text = (jstring)jenv->CallObjectMethod(objForString, midStringToString, NULL);
    pstem = jenv->GetStringUTFChars(text, NULL);

    if (pstem == NULL) {
        ERROR("Out of memory error in JNI");
    }

    //jenv->ReleaseStringUTFChars(text, pstem);
    return pstem;
}

const char* Morfologik::getTagsByJNI(jobject objWordData) {
    const char *ptags = NULL;

    jobject objForString = jenv->CallObjectMethod(objWordData, midWordDataGetTag, NULL);
    jstring text = (jstring)jenv->CallObjectMethod(objForString, midStringToString, NULL);
    ptags = jenv->GetStringUTFChars(text, NULL);

    if (ptags == NULL) {
        ERROR("Out of memory error in JNI");
    }

    //jenv->ReleaseStringUTFChars(text, ptags);
    return ptags;
}

void Morfologik::initializeDictionary() {
    enmDictionary = NULL;
    fidDictionary = NULL;
    objDictionary = NULL;

    enmDictionary = jenv->FindClass("morfologik/stemming/PolishStemmer$DICTIONARY");

    if (enmDictionary != NULL) {
        fidDictionary = jenv->GetStaticFieldID(enmDictionary, dictionary_.c_str(),
            "Lmorfologik/stemming/PolishStemmer$DICTIONARY;");
        if (jenv->ExceptionCheck()) jenv->ExceptionDescribe();

        objDictionary = jenv->GetStaticObjectField(enmDictionary, fidDictionary);
        if (jenv->ExceptionCheck()) jenv->ExceptionDescribe();
    }
    else {
        ERROR("The DICTIONARY enum has been not found");
    }
}

void Morfologik::initializePolishStemmer() {
    clsPolishStemmer = NULL;
    objPolishStemmer = NULL;
    midPolishStemmerConstructor = NULL;
    midPolishStemmerLookup = NULL;

    clsPolishStemmer = jenv->FindClass("morfologik/stemming/PolishStemmer");

    if (clsPolishStemmer != NULL) {
        midPolishStemmerConstructor = jenv->GetMethodID
            (clsPolishStemmer, "<init>", "(Lmorfologik/stemming/PolishStemmer$DICTIONARY;)V");
        midPolishStemmerLookup = jenv->GetMethodID
            (clsPolishStemmer, "lookup", "(Ljava/lang/CharSequence;)Ljava/util/List;");
    }
    else {
        ERROR("The PolishStemmer class from morfologik's jar file has been not found");
    }

    objPolishStemmer = jenv->NewObject
        (clsPolishStemmer, midPolishStemmerConstructor, objDictionary);
    if (jenv->ExceptionCheck()) jenv->ExceptionDescribe();
}

void Morfologik::initializeList() {
    clsList = NULL;
    midListGetSize = NULL;
    midListGetElement = NULL;

    clsList = jenv->FindClass("java/util/List");
    if (clsList != NULL) {
        midListGetSize = jenv->GetMethodID(clsList, "size", "()I");
        midListGetElement = jenv->GetMethodID(clsList, "get", "(I)Ljava/lang/Object;");
    }
    else {
        ERROR("The List class has been not found");
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
    else {
        ERROR("The WordData class from morfologik's jar file has been not found");
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
        ERROR("The String class has been not found");
    }
}
