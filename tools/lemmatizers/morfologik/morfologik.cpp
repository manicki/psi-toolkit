#include "morfologik.hpp"
#include "logging.hpp"
#include "config.hpp"

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/assign.hpp>

const std::string Morfologik::TAGS_SEPARATOR = "+";

const std::vector<std::string> Morfologik::DICTIONARIES = boost::assign::list_of
    ("MORFOLOGIK")/*("MORFEUSZ")*/("COMBINED");

Morfologik::Morfologik(const boost::program_options::variables_map& options)
    : level_(3), dictionary_("MORFOLOGIK"), annotationManager_(NULL)
{
    if (options.count("level") > 0) setLevel(options["level"].as<int>());
    if (options.count("dict") > 0) setDictionary(options["dict"].as<std::string>());

    jenv_ = NULL;
    JavaVirtualMachine *jvm = JavaVirtualMachine::Instance();
    jenv_ = jvm->getENV();

    initializeDictionary_();
    initializePolishStemmer_();
    initializeList_();
    initializeWordData_();
    initializeString_();
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
        level_ = lvl;
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

void Morfologik::lemmatize(const std::string & word, AnnotationItemManager & manager,
    LemmatizerOutputIterator & iterator) {

    annotationManager_ = &manager;

    switch (level_) {
        case 0:
            break;
        case 1:
            stemsOnLemmaLevel_(word, iterator);
            break;
        case 2:
            stemsOnLexemeLevel_(word, iterator);
            break;
        default:
            stemsOnFormLevel_(word, iterator);
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

void Morfologik::stemsOnLemmaLevel_(const std::string & word,
    LemmatizerOutputIterator & outputIterator) {

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

void Morfologik::stemsOnLexemeLevel_(const std::string & word,
    LemmatizerOutputIterator & outputIterator) {

    std::multimap<std::string, std::vector<std::string> > stems = stem(word);

    std::set<std::string> lemmas = getLemmasFromStems_(stems);
    std::set<std::string>::iterator lem;

    DEBUG("found stems of word [" << word << "] on lexeme level: ["
        << boost::algorithm::join(lemmas, ", ") << "]");

    for (lem = lemmas.begin(); lem != lemmas.end(); ++lem) {
        outputIterator.addLemma(*lem);

        std::vector<std::string> lexemeTags = getLexemeTagsFromStems_(stems, *lem);
        std::vector<std::string>::iterator lxt;

        for (lxt = lexemeTags.begin(); lxt != lexemeTags.end(); ++lxt) {
            AnnotationItem lexItem = createLexemeAnnotation_(*lem, *lxt);
            outputIterator.addLexeme(lexItem);
        }
    }
}

std::set<std::string> Morfologik::getLemmasFromStems_(
    std::multimap<std::string, std::vector<std::string> > stems) {

    std::multimap<std::string, std::vector<std::string> >::iterator s;
    std::set<std::string> lemmas;

    for (s = stems.begin(); s != stems.end(); ++s) {
        lemmas.insert(lemmas.end(), s->first);
    }

    return lemmas;
}

std::vector<std::string> Morfologik::getLexemeTagsFromStems_(
    std::multimap<std::string, std::vector<std::string> > & stems, const std::string & lemma) {

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

AnnotationItem Morfologik::createLexemeAnnotation_(const std::string& stem, std::string& tag) {

    std::map<std::string, std::string> attributes = tagsParser_.getLexemeAttributes(tag);
    std::map<std::string, std::string>::iterator atr;

    std::string partOfSpeech = attributes["pos"];
    std::string wordId = stem + LEMMA_CATEGORY_SEPARATOR + partOfSpeech;
    attributes.erase("pos");

    AnnotationItem lexeme(partOfSpeech, StringFrag(wordId));

    for (atr = attributes.begin(); atr != attributes.end(); ++atr) {
        annotationManager_->setValue(lexeme, atr->first, atr->second);
    }

    return lexeme;
}

void Morfologik::stemsOnFormLevel_(const std::string & word,
    LemmatizerOutputIterator & outputIterator) {

    std::multimap<std::string, std::vector<std::string> > stems = stem(word);

    std::set<std::string> lemmas = getLemmasFromStems_(stems);
    std::set<std::string>::iterator lem;

    DEBUG("found stems of word [" << word << "] on form level: ["
        << boost::algorithm::join(lemmas, ", ") << "]");

    for (lem = lemmas.begin(); lem != lemmas.end(); ++lem) {
        outputIterator.addLemma(*lem);

        std::multimap<std::string, std::vector<std::string> >::iterator lex;

        for (lex = stems.equal_range(*lem).first; lex != stems.equal_range(*lem).second; ++lex) {
            std::vector<std::string> tags = lex->second;
            std::vector<std::string>::iterator tag = tags.begin();

            AnnotationItem lexItm = createLexemeAnnotation_(*lem, *tag);
            outputIterator.addLexeme(lexItm);

            for (tag = tags.begin(); tag != tags.end(); ++tag) {

                std::vector<std::map<std::string, std::string> > forms =
                    tagsParser_.getFormAttributes(*tag);
                std::vector<std::map<std::string, std::string> >::iterator frm;

                for (frm = forms.begin(); frm != forms.end(); ++frm) {
                    AnnotationItem frmItm = createFormAnnotation_(lexItm, word, *frm);
                    outputIterator.addForm(frmItm);
                }
            }

        }
    }
}

AnnotationItem Morfologik::createFormAnnotation_(AnnotationItem & lexemeItem,
    const std::string& word, std::map<std::string, std::string> & attributes) {

    AnnotationItem formItem(lexemeItem, word);

    std::map<std::string, std::string>::iterator atr;
    for (atr = attributes.begin(); atr != attributes.end(); ++atr) {
        annotationManager_->setValue(formItem, atr->first, atr->second);
    }
    return formItem;
}

std::vector<std::string> Morfologik::simpleStem(const std::string & word) {
    jstring jword = jenv_->NewStringUTF(word.c_str());
    jobject objList = (jobject)jenv_->CallObjectMethod
        (objPolishStemmer_, midPolishStemmerLookup_, jword);
    jenv_->DeleteLocalRef(jword);

    int stemsCount = (int)jenv_->CallIntMethod(objList, midListGetSize_, NULL);
    jobject objWordData = NULL;
    const char *pstem = NULL;
    std::vector<std::string> result;

    for (int i = 0; i < stemsCount; ++i) {
        objWordData = (jobject)jenv_->CallObjectMethod(objList, midListGetElement_, (jint)i);
        pstem = getStemByJNI_(objWordData);
        result.insert(result.begin(), pstem);
    }

    jenv_->DeleteLocalRef(objList);
    jenv_->DeleteLocalRef(objWordData);

    return result;
}

std::multimap<std::string, std::vector<std::string> > Morfologik::stem(const std::string & word) {
    jstring jword = jenv_->NewStringUTF(word.c_str());
    jobject objList = (jobject)jenv_->CallObjectMethod
        (objPolishStemmer_, midPolishStemmerLookup_, jword);
    jenv_->DeleteLocalRef(jword);

    int stemsCount = (int)jenv_->CallIntMethod(objList, midListGetSize_, NULL);
    DEBUG("Found " << stemsCount << " stems for word " << word);

    jobject objWordData = NULL;
    const char *pstem = NULL;
    const char *ptags = NULL;
    std::multimap<std::string, std::vector<std::string> > result;
    std::vector<std::string> tags;

    for (int i = 0; i < stemsCount; ++i) {
        objWordData = (jobject)jenv_->CallObjectMethod(objList, midListGetElement_, (jint)i);

        pstem = getStemByJNI_(objWordData);
        ptags = getTagsByJNI_(objWordData);

        tags.clear();
        boost::split(tags, ptags, boost::is_any_of(TAGS_SEPARATOR));
        result.insert(std::pair<std::string, std::vector<std::string> > (pstem, tags));
    }

    jenv_->DeleteLocalRef(objList);
    jenv_->DeleteLocalRef(objWordData);
    return result;
}

const char* Morfologik::getStemByJNI_(jobject objWordData) {
    const char *pstem = NULL;

    jobject objForString = jenv_->CallObjectMethod(objWordData, midWordDataGetStem_, NULL);
    jstring text = (jstring)jenv_->CallObjectMethod(objForString, midStringToString_, NULL);
    pstem = jenv_->GetStringUTFChars(text, NULL);

    if (pstem == NULL) {
        ERROR("Out of memory error in JNI");
    }

    //jenv_->ReleaseStringUTFChars(text, pstem);
    return pstem;
}

const char* Morfologik::getTagsByJNI_(jobject objWordData) {
    const char *ptags = NULL;

    jobject objForString = jenv_->CallObjectMethod(objWordData, midWordDataGetTag_, NULL);
    jstring text = (jstring)jenv_->CallObjectMethod(objForString, midStringToString_, NULL);
    ptags = jenv_->GetStringUTFChars(text, NULL);

    if (ptags == NULL) {
        ERROR("Out of memory error in JNI");
    }

    //jenv_->ReleaseStringUTFChars(text, ptags);
    return ptags;
}

void Morfologik::initializeDictionary_() {
    enmDictionary_ = NULL;
    fidDictionary_ = NULL;
    objDictionary_ = NULL;

    enmDictionary_ = jenv_->FindClass("morfologik/stemming/PolishStemmer$DICTIONARY");

    if (enmDictionary_ != NULL) {
        fidDictionary_ = jenv_->GetStaticFieldID(enmDictionary_, dictionary_.c_str(),
            "Lmorfologik/stemming/PolishStemmer$DICTIONARY;");
        if (jenv_->ExceptionCheck()) jenv_->ExceptionDescribe();

        objDictionary_ = jenv_->GetStaticObjectField(enmDictionary_, fidDictionary_);
        if (jenv_->ExceptionCheck()) jenv_->ExceptionDescribe();
    }
    else {
        ERROR("The DICTIONARY enum has been not found");
    }
}

void Morfologik::initializePolishStemmer_() {
    clsPolishStemmer_ = NULL;
    objPolishStemmer_ = NULL;
    midPolishStemmerConstructor_ = NULL;
    midPolishStemmerLookup_ = NULL;

    clsPolishStemmer_ = jenv_->FindClass("morfologik/stemming/PolishStemmer");

    if (clsPolishStemmer_ != NULL) {
        midPolishStemmerConstructor_ = jenv_->GetMethodID
            (clsPolishStemmer_, "<init>", "(Lmorfologik/stemming/PolishStemmer$DICTIONARY;)V");
        midPolishStemmerLookup_ = jenv_->GetMethodID
            (clsPolishStemmer_, "lookup", "(Ljava/lang/CharSequence;)Ljava/util/List;");
    }
    else {
        ERROR("The PolishStemmer class from morfologik's jar file has been not found");
    }

    objPolishStemmer_ = jenv_->NewObject
        (clsPolishStemmer_, midPolishStemmerConstructor_, objDictionary_);
    if (jenv_->ExceptionCheck()) jenv_->ExceptionDescribe();
}

void Morfologik::initializeList_() {
    clsList_ = NULL;
    midListGetSize_ = NULL;
    midListGetElement_ = NULL;

    clsList_ = jenv_->FindClass("java/util/List");

    if (clsList_ != NULL) {
        midListGetSize_ = jenv_->GetMethodID(clsList_, "size", "()I");
        midListGetElement_ = jenv_->GetMethodID(clsList_, "get", "(I)Ljava/lang/Object;");
    }
    else {
        ERROR("The List class has been not found");
    }
}

void Morfologik::initializeWordData_() {
    clsWordData_ = NULL;
    midWordDataGetStem_ = NULL;
    midWordDataGetTag_ = NULL;

    clsWordData_ = jenv_->FindClass("morfologik/stemming/WordData");

    if (clsWordData_ != NULL) {
        midWordDataGetStem_ = jenv_->GetMethodID
            (clsWordData_, "getStem", "()Ljava/lang/CharSequence;");
        midWordDataGetTag_ = jenv_->GetMethodID
            (clsWordData_, "getTag", "()Ljava/lang/CharSequence;");
    }
    else {
        ERROR("The WordData class from morfologik's jar file has been not found");
    }
}

void Morfologik::initializeString_() {
    clsString_ = NULL;
    midStringToString_ = NULL;

    clsString_ = jenv_->FindClass("java/lang/String");

    if (clsString_ != NULL) {
        midStringToString_ = jenv_->GetMethodID(clsString_, "toString", "()Ljava/lang/String;");
    }
    else {
        ERROR("The String class has been not found");
    }
}
