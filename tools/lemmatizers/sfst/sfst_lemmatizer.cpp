#include "sfst_lemmatizer.hpp"

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>

std::string SfstLemmatizer::tagSeparator = ":";

SfstLemmatizer::SfstLemmatizer(const boost::program_options::variables_map& options)
    : annotationManager(NULL), level(3)
{
    if (options.count("level") > 0) {
        setLevel(options["level"].as<int>());
    }

    if (options.count("lang") > 0) {
        setLanguage(options["lang"].as<std::string>());
    }
    
    if (options.count("automaton") > 0) {
        setAutomaton(options["automaton"].as<std::string>());
    }

    initializeTransducer();
}
boost::filesystem::path SfstLemmatizer::getFile() {
    return __FILE__;
}

std::list<std::string> SfstLemmatizer::getLayerTags() {
    std::list<std::string> layerTags;

    layerTags.push_back("sfst");
    layerTags.push_back("sfst-tagset");

    return layerTags;
}

boost::program_options::options_description SfstLemmatizer::optionsHandled() {
    boost::program_options::options_description desc("Allowed options");

    desc.add_options()
        ("level", boost::program_options::value<int>()->default_value(1),
            "set word processing level 0-3")
        ("lang", boost::program_options::value<std::string>()->default_value("tr"),
            "set default language")
        ("automaton", boost::program_options::value<std::string>(),
            "set file name");

    return desc;
}

std::string SfstLemmatizer::getName() {
    return "sfst-Lemmatizer";
}

std::string SfstLemmatizer::getLanguage() const {
    return language;
}

AnnotatorFactory::LanguagesHandling SfstLemmatizer::languagesHandling(
    const boost::program_options::variables_map& /*options*/) {
    return AnnotatorFactory::JUST_ONE_LANGUAGE;
}

std::list<std::string> SfstLemmatizer::languagesHandled(
    const boost::program_options::variables_map& /*options*/) {
    return boost::assign::list_of(std::string("tr"));
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

//Finished - adds unique stems to lemmas list
void SfstLemmatizer::stemsOnLemmaLevel(
    const std::string & word, LemmatizerOutputIterator & outputIterator
) {

    std::vector<std::string> stems = simpleStem(word);
    

    for (std::vector<std::string>::iterator i = stems.begin(); i != stems.end(); ++i) {
        std::string stem = *(i);
        outputIterator.addLemma(stem);
    }
}
//partialy...
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
//WOrks... ? :P
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
//should work as in morfo...
std::vector<std::string> SfstLemmatizer::getLexemeTagsFromStems(
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

//should work as in morfo...
std::vector<std::string> SfstLemmatizer::getFormTagsFromLexem(
    std::multimap<std::string, std::vector<std::string> > & stems,
    const std::string & lemma,
    const std::string & lexem
) {
    std::vector<std::string> tags;
    std::vector<std::string>::iterator t;

    std::multimap<std::string, std::vector<std::string> >::iterator s;
    for (s = stems.begin(); s != stems.end(); ++s) {
		if (s->first == lemma){
			if (*(s->second).begin() == lexem) {
				t = (s->second).begin();
				for (t++; t != (s->second).end(); ++t) {
					tags.insert(tags.begin(), *t);
				}
			}
		}
    }

    return tags;
}

AnnotationItem SfstLemmatizer::createLexemeAnnotation(
    const std::string & stem, std::string & tag
) {
//UWAGA! przygotować tabelę dla różnych atrybutów
    //std::map<std::string, std::string> attributes =
        //tagsParser.getLexemeAttributes(tag);
    //std::map<std::string, std::string>::iterator atr;

    //std::string partOfSpeech = attributes["pos"];
    std::string wordId = stem + LEMMA_CATEGORY_SEPARATOR + tag;//partOfSpeech;
    //attributes.erase("pos");

    AnnotationItem lexeme(/*partOfSpeech*/tag, StringFrag(wordId));

    //for (atr = attributes.begin(); atr != attributes.end(); ++atr) {
        annotationManager->setValue(lexeme, tag, "pos");
    //}
    return lexeme;
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

        std::vector<std::string> lexemeTags =
            getLexemeTagsFromStems(stems, *lem);
        std::vector<std::string>::iterator lxt;

        for (lxt = lexemeTags.begin(); lxt != lexemeTags.end(); ++lxt) {
            AnnotationItem lexItem = createLexemeAnnotation(*lem, *lxt);
            outputIterator.addLexeme(lexItem);
            
            std::vector<std::string> formTags = getFormTagsFromLexem(stems, *lem, *lxt);
            
            
			std::vector<std::string>::iterator frm;
            
            for (frm = formTags.begin(); frm != formTags.end(); ++frm) {
                    AnnotationItem frmItm = createFormAnnotation(lexItem, *lem, *frm);
                    outputIterator.addForm(frmItm);
			}
        }
    }
/*will wait with this for tests...
    
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
    */
}

void SfstLemmatizer::setLevel(int lvl) {
    if (0 <= lvl && lvl <= 3) {
        level = lvl;
    }
}

void SfstLemmatizer::setLanguage(std::string lang) {
    if (lang.length() == 2) {
        language = lang;
    }
}

void SfstLemmatizer::setAutomaton(std::string automa) {
    //if (boost::trim(automa) != "") {
        automaton = automa;
    //}
}

std::vector<std::string> SfstLemmatizer::wordToRaw(std::string word) {
	
	std::vector<std::string> result;
	
	char *buffer;
	size_t buffer_size;
	FILE* memory_file = open_memstream(&buffer, &buffer_size);
	
	
	
	if (!transducer->analyze_string((char*)word.c_str(), memory_file, true)) {
            return result;
        } else {            
		fclose (memory_file); //fflush for further read/write
					
		boost::split(result, buffer, boost::is_any_of("\n") );
	}  
	return result;
}

void SfstLemmatizer::cookRaw(std::string & word) {
	boost::trim(word);
	boost::replace_first(word, "<", ":");
	boost::erase_last(word, ">");
	boost::replace_all(word, "><", ":");
}

std::string SfstLemmatizer::getCookedStem(std::string word){
	
	std::vector<std::string> tags;
	std::string stem;

	boost::split(tags, word, boost::is_any_of(tagSeparator));

	stem = *(tags.begin());
	
	return stem;
};

std::vector<std::string> SfstLemmatizer::getCookedTags(std::string word){
	std::vector<std::string> tags;
	std::string stem;
	
	boost::split(tags, word, boost::is_any_of(tagSeparator));
	
	tags.erase(tags.begin());
	
	return tags;
};

void SfstLemmatizer::initializeTransducer() {
    FILE *file;

	//std::string file_name = "data/"+language+"/sfst-"+language+".a";
	std::string file_name = automaton;
	
    if ((file = fopen(file_name.c_str(),"rb")) == NULL) {
        ERROR("Proper lang file (" + language + ") has not been found.");        
	}
    else {
		transducer = new SFST::Transducer(file);
        fclose(file);
    }
	
}

AnnotationItem SfstLemmatizer::createFormAnnotation(
    AnnotationItem & lexemeItem,
    const std::string & word,
    std::string & attributes
) {

    AnnotationItem formItem(lexemeItem, word);

    //std::vector<std::string>::iterator atr;
    //for (atr = attributes.begin(); atr != attributes.end(); ++atr) {
        annotationManager->setValue(formItem, attributes, "yes");
    //}
    return formItem;
}

std::vector<std::string> SfstLemmatizer::simpleStem(const std::string & word) {

    std::vector<std::string> result;

	std::vector<std::string> raw = wordToRaw(word);
	
	for(vector<std::string>::iterator i = raw.begin(); i != raw.end(); ++i) {
		cookRaw(*(i));
		if (*(i) != "") {
				result.push_back(getCookedStem(*(i)));
		} 
	}
    
        
    boost::sort(result);
    boost::erase_range(result, boost::unique<boost::return_found_end>(result));
    
    return result;
}

//Finished - return what it should from raw data
std::multimap<std::string, std::vector<std::string> > SfstLemmatizer::stem(
    const std::string & word
) {
	
    std::multimap<std::string, std::vector<std::string> > result;
    std::vector<std::string> tags;
    
	std::vector<std::string> raw = wordToRaw(word);
	
	for(vector<std::string>::iterator i = raw.begin(); i != raw.end(); ++i) {
		cookRaw(*(i));
		if (*(i) != "") {
				result.insert(std::pair<std::string, std::vector<std::string> >
					(getCookedStem(*(i)), getCookedTags(*(i))) );
		} 
	}
    
    return result;
}
