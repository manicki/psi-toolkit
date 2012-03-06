#include "sfst_lemmatizer.hpp"

#include <iostream>
#include <boost/algorithm/string.hpp>

std::string SfstLemmatizer::tagSeparator = "+";

SfstLemmatizer::SfstLemmatizer(const boost::program_options::variables_map& options)
    : annotationManager(NULL), level(3)
{
    if (options.count("level") > 0) {
        setLevel(options["level"].as<int>());
    }

    initializeTurkishTransducer();
    //initializeList();
    //initializeWordData();
    //initializeString();
}

std::string SfstLemmatizer::getName() {
    return "sfstLemmatizer";
}

std::string SfstLemmatizer::getLanguage() const {
    return "tr";
}

void SfstLemmatizer::lemmatize(const std::string & word,
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
//todo
void SfstLemmatizer::stemsOnLemmaLevel(
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
//todo
void SfstLemmatizer::stemsOnLexemeLevel(
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
//todo
std::set<std::string> SfstLemmatizer::getLemmasFromStems(
    std::multimap<std::string, std::vector<std::string> > stems
) {
    std::set<std::string> lemmas;

    std::multimap<std::string, std::vector<std::string> >::iterator s;
    for (s = stems.begin(); s != stems.end(); ++s) {
        lemmas.insert(lemmas.end(), s->first);
    }

    return lemmas;
}

void SfstLemmatizer::stemsOnFormLevel(
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
                    AnnotationItem frmItm = createFormAnnotation(lexItm, word, *frm);
                    outputIterator.addForm(frmItm);
                }
            }

        }
    }
}

AnnotationItem SfstLemmatizer::createFormAnnotation(
    AnnotationItem & lexemeItem,
    const std::string& word,
    std::map<std::string, std::string> & attributes
) {

    AnnotationItem formItem(lexemeItem, word);

    std::map<std::string, std::string>::iterator atr;
    for (atr = attributes.begin(); atr != attributes.end(); ++atr) {
        annotationManager->setValue(formItem, atr->first, atr->second);
    }
    return formItem;
}

void SfstLemmatizer::setLevel(int lvl) {
    if (0 <= lvl && lvl <= 3) {
        level = lvl;
    }
}

void SfstLemmatizer::initializeTurkishTransducer() {
    FILE *file;
    
    //turkishTransducer = NULL;

    if ((file = fopen("data/tr/trmorph-0.2.1.a","rb")) == NULL) {
        SFST::Transducer tmpTrans(file);
        fclose(file);
		turkishTransducer = tmpTrans;
    }
    else {
        ERROR("The trmormp's file has been not found");
    }
	
}

std::vector<std::string> SfstLemmatizer::simpleStem(const std::string & word) {

    std::vector<std::string> result;

	std::streambuf* oldBuf = std::cout.rdbuf();
	std::ostringstream stdoutCap;
	std::cout.rdbuf( stdoutCap.rdbuf() );
    
	if (turkishTransducer.analyze_string((char*)word.c_str(), stdout, true)) {
		
		size_t pos;
		
		std::string output = stdoutCap.str();

		while((pos = output.find_first_of('\n')) != -1) {
			std::string stem = output.substr(0, pos);
			output = output.substr(pos + 1);
			if ((pos = stem.find_first_of('<')) == -1)
				result.insert(result.begin(), stem);
			else 
				result.insert(result.begin(), stem.substr(0, pos));
			}
		
	}
    
	std::cout.rdbuf( oldBuf );
    
    return result;
}
//todo
std::multimap<std::string, std::vector<std::string> > SfstLemmatizer::stem(
    const std::string & word
) {
	
    std::multimap<std::string, std::vector<std::string> > result;
    std::vector<std::string> tags;
	
/*oldie
    jstring jword = jenv->NewStringUTF(word.c_str());
    jobject objList = (jobject)jenv->CallObjectMethod
        (objPolishStemmer, midPolishStemmerLookup, jword);
    jenv->DeleteLocalRef(jword);

    int stemsCount = (int)jenv->CallIntMethod(objList, midListGetSize, NULL);

    jobject objWordData = NULL;
    const char *pstem = NULL;
    const char *ptags = NULL;

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
    * */
    return result;
}
