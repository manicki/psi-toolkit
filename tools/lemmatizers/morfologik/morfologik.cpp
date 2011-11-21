#include "morfologik.hpp"
#include "logging.hpp"

#include <iostream>
#include <boost/algorithm/string.hpp>

std::string Morfologik::tagSeparator = "+";

Morfologik::Morfologik(const boost::program_options::variables_map& options)
    : annotationManager(NULL)
{
    jenv = NULL;
    level = 3;

    JavaVirtualMachine *jvm = JavaVirtualMachine::Instance();
    jenv = jvm->getENV();

    initializePolishStemmer();
    initializeList();
    initializeWordData();
    initializeString();
}

std::string Morfologik::getName() {
    return "morfologik";
}

std::list<std::string> Morfologik::getLayerTags() {
    std::list<std::string> layerTags;

    layerTags.push_back("morfologik");
    layerTags.push_back("morfologik-tagset");

    return layerTags;
}

void Morfologik::setLevel(int lvl) {
    level = lvl;
}

void Morfologik::lemmatize(const std::string & word,
    AnnotationItemManager & manager, LemmatizerOutputIterator & iterator
) {
    annotationManager = &manager;

    switch(level) {
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
    boost::program_options::options_description desc(
        //"Morfologik is a Polish morphological analyzer written in Java.\n"
        //"\n"
        "Allowed options:"
    );

    return desc;
}

void Morfologik::stemsOnLemmaLevel(
    const std::string & word, LemmatizerOutputIterator & outputIterator
) {

    std::vector<std::string> stems = simpleStem(word);
    std::vector<std::string>::iterator i;

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

    std::multimap<std::string, std::vector<std::string> > stems =
        stem(word);

    std::set<std::string> lemmas = getLemmasFromStems(stems);
    std::set<std::string>::iterator lem;

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

AnnotationItem Morfologik::createLexemeAnnotation(
    const std::string & stem, std::string & tag
) {

    std::map<std::string, std::string> attributes =
        tagsParser.getLexemeAttributes(tag);
    std::map<std::string, std::string>::iterator atr;

    std::string partOfSpeech = attributes["pos"];
    std::string wordId = stem + "_" + partOfSpeech;
    attributes.erase("pos");

    AnnotationItem lexeme(partOfSpeech, wordId);

    for (atr = attributes.begin(); atr != attributes.end(); ++atr) {
        annotationManager->setValue(lexeme, atr->first, atr->second);
    }
    return lexeme;
}

void Morfologik::stemsOnFormLevel(
    const std::string & word, LemmatizerOutputIterator & outputIterator
){

    std::multimap<std::string, std::vector<std::string> > stems =
        stem(word);

    std::set<std::string> lemmas = getLemmasFromStems(stems);
    std::set<std::string>::iterator lem;

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
                    AnnotationItem frmItm = createFormAnnotation(lexItm, *frm);
                    outputIterator.addForm(frmItm);
                }
            }

        }
    }
}

AnnotationItem Morfologik::createFormAnnotation(
    AnnotationItem & lexemeItem, std::map<std::string, std::string> & attributes
) {

    AnnotationItem formItem = lexemeItem;

    std::map<std::string, std::string>::iterator atr;
    for (atr = attributes.begin(); atr != attributes.end(); ++atr) {
        annotationManager->setValue(lexemeItem, atr->first, atr->second);
    }
    return lexemeItem;
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
        objWordData = (jobject)jenv->CallObjectMethod
            (objList, midListGetElement, (jint)i);

        pstem = getStemByJNI(objWordData);
        result.insert(result.begin(), pstem);
    }

    jenv->DeleteLocalRef(objList);
    jenv->DeleteLocalRef(objWordData);
    return result;
}

std::multimap<std::string, std::vector<std::string> > Morfologik::stem(
    const std::string & word
) {

    jstring jword = jenv->NewStringUTF(word.c_str());
    jobject objList = (jobject)jenv->CallObjectMethod
        (objPolishStemmer, midPolishStemmerLookup, jword);
    jenv->DeleteLocalRef(jword);

    int stemsCount = (int)jenv->CallIntMethod(objList, midListGetSize, NULL);

    jobject objWordData = NULL;
    const char *pstem = NULL;
    const char *ptags = NULL;
    std::multimap<std::string, std::vector<std::string> > result;
    std::vector<std::string> tags;

    for (int i = 0; i < stemsCount; ++i) {
        objWordData = (jobject)jenv->CallObjectMethod
            (objList, midListGetElement, (jint)i);

        pstem = getStemByJNI(objWordData);
        ptags = getTagsByJNI(objWordData);

        tags.clear();
        boost::split(tags, ptags, boost::is_any_of(tagSeparator));
        result.insert(std::pair<std::string, std::vector<std::string> >
            (pstem, tags) );
    }

    jenv->DeleteLocalRef(objList);
    jenv->DeleteLocalRef(objWordData);
    return result;
}

const char* Morfologik::getStemByJNI(jobject objWordData) {

    const char *pstem = NULL;

    jobject objForString = jenv->CallObjectMethod
        (objWordData, midWordDataGetStem, NULL);
    jstring text = (jstring)jenv->CallObjectMethod
        (objForString, midStringToString, NULL);
    pstem = jenv->GetStringUTFChars(text, NULL);

    if (pstem == NULL) {
        // OutOfMemoryError already throw
    }

    //jenv->ReleaseStringUTFChars(text, pstem);
    return pstem;
}

const char* Morfologik::getTagsByJNI(jobject objWordData) {

    const char *ptags = NULL;

    jobject objForString = jenv->CallObjectMethod(
        objWordData, midWordDataGetTag, NULL);
    jstring text = (jstring)jenv->CallObjectMethod
        (objForString, midStringToString, NULL);
    ptags = jenv->GetStringUTFChars(text, NULL);

    if (ptags == NULL) {
        // OutOfMemoryError already throw
    }

    //jenv->ReleaseStringUTFChars(text, ptags);
    return ptags;
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
        ERROR("The PolishStemmer class from morfologik's jar file has been not found");
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

