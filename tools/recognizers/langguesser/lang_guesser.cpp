/*
  Copyright (C) 2007-2011 Poleng Sp. z o.o.

  This file is part of Translatica language identification module.

  Translatica language identification module (along with bigram
  tables) can be redistributed and/or modified under the terms of the
  GNU Lesser General Public Licence as published by the Free Software
  Foundation.
*/

#include "lang_guesser.hpp"

#include "lattice.hpp"
#include "logging.hpp"

#include <boost/assign.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>

#define LOWER_LETTER(i) ( ((i) > 90 || (i) < 65) ? (i) : ((i) + 32) )

std::string LangGuesser::UNKNOWN_LANGUAGE = "unknown";
std::string LangGuesser::DEFAULT_DEFAULT_LANGUAGE = "xx";
std::string LangGuesser::NONE_LANGUAGE = "none";

std::map<std::string, std::string> LangGuesser::LANGUAGES =
    boost::assign::map_list_of
        ("pl", "ąćęłńóśźżĄĆĘŁŃÓŚŹŻ")
        ("en", "")
        ("de", "äöüßÄÖÜ")
        ("ru", "абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ")
        ("fi", "äöå")
        ("sv", "äöå") ;

LangGuesser::LangGuesser() {
    initLanguages();
}

LangGuesser::LangGuesser(const boost::program_options::variables_map& options)
    : forceMode_(false), defaultLanguage_(DEFAULT_DEFAULT_LANGUAGE) {

    forceMode_ = options.count("force") ? true : false;

    if (options.count("default-language"))
        defaultLanguage_ = options["default-language"].as<std::string>();

    if (options.count("only-langs")) {
        std::vector<std::string> langsWanted =
            options["only-langs"].as<std::vector<std::string> >();

        if (forceMode_ && langsWanted.size() == 1) {
            forcedLanguage_ = langsWanted[0];
            initLanguages();
        } else
            initLanguages(langsWanted);
    }
    else
        initLanguages();
}

void LangGuesser::initLanguages() {
    std::map<std::string, std::string>::iterator it;

    for (it = LANGUAGES.begin(); it != LANGUAGES.end(); ++it) {
        addLanguage(it->first, it->second);
    }
}

void LangGuesser::initLanguages(std::vector<std::string> selectedLangs) {
    std::map<std::string, std::string>::iterator definedLang;

    BOOST_FOREACH(const std::string& selectedLang, selectedLangs) {
        definedLang = LANGUAGES.find(selectedLang);

        if (definedLang != LANGUAGES.end())
            addLanguage(definedLang->first, definedLang->second);
        else
            throw Exception(
                std::string("lang-guesser cannot recognize language '")
                + selectedLang
                + "'");
    }

    if (languages_.empty()) {
        initLanguages();
    }
}

void LangGuesser::addLanguage(std::string lang, std::string letters) {
    static ProcessorFileFetcher fileFetcher(__FILE__);

    languages_.push_back(
        Language(lang, fileFetcher.getOneFile("%ITSDATA%/" + lang + "lang.i"), letters)
     );
}

std::string LangGuesser::guessLanguage(std::string text) {
    if (!forcedLanguage_.empty())
        return forcedLanguage_;

    std::string guessedLanguage = (text.length() < MIN_TEXT_LENGTH_FOR_BIGRAM_METHOD) ?
        guessLanguageByLetters(text) : guessLanguageByBigramModel(text);

    if ((guessedLanguage == UNKNOWN_LANGUAGE) && forceMode_) {
        if (languages_.size() == 1) {
            guessedLanguage = languages_.front().name;
        }
        else {
            if (text.length() < MIN_TEXT_LENGTH_FOR_BIGRAM_METHOD) {
                guessedLanguage = guessLanguageByBigramModel(text);
                if (guessedLanguage == UNKNOWN_LANGUAGE)
                    guessedLanguage = languages_.front().name;
            }
        }
    }

    if (guessedLanguage == UNKNOWN_LANGUAGE && defaultLanguage_ != NONE_LANGUAGE)
        guessedLanguage = defaultLanguage_;

    return guessedLanguage;
}

std::string LangGuesser::guessLanguageByBigramModel(std::string text) {
    if (text.length() == 0) {
        return UNKNOWN_LANGUAGE;
    }

    std::string theBestLang = UNKNOWN_LANGUAGE;

    BigramLanguageModel inputTextBigramModel(text);

    double dist;
    double minDist = 100000000000.0;
    std::stringstream debugInfo;

    BOOST_FOREACH (Language lang, languages_) {
        dist = distance(inputTextBigramModel.frequencyTable(), lang.model.frequencyTable());
        debugInfo << lang.name << " -> " << dist << ", ";

        if (dist < minDist) {
            theBestLang = lang.name;
            minDist = dist;
        }
    }

    DEBUG("LangGuesser debug: " << debugInfo.str());

    return theBestLang;
}

std::string LangGuesser::guessLanguageByLetters(std::string text) {

    BOOST_FOREACH (Language lang, languages_) {

        if (lang.letters.length()) {
            bool found = false;
            bool allWords = true;
            bool foundInWord = false;
            bool isWord = false;

            std::string::iterator iter = text.begin();
            std::string::iterator end = text.end();

            while (iter != end) {
                utf8::uint32_t letter = utf8::next(iter, end);

                if (isOneOfTheLanguageSpecificLetters(letter, lang.letters)) {
                    foundInWord = true;
                    found = true;
                }

                if (letter == ' ') {
                    if (isWord && !foundInWord) {
                        allWords = false;
                        found = false;
                    }
                    foundInWord = false;
                    isWord = false;
                }
                else {
                    isWord = true;
                }
            }

            // if there is no language specific letter within the last word
            if (isWord && !foundInWord) {
                found = false;
            }

            if (found && allWords) {
                return lang.name;
            }

        }
    }

    return UNKNOWN_LANGUAGE;
}

bool LangGuesser::isOneOfTheLanguageSpecificLetters(utf8::uint32_t letter, std::string& letters) {
    std::string::iterator iter = letters.begin();
    std::string::iterator end = letters.end();

    while (iter != end) {
        utf8::uint32_t langSpecificLetter = utf8::next(iter, end);
        if (letter == langSpecificLetter) {
            return true;
        }
    }

    return false;
}

double LangGuesser::distance(double* ftableOne, double* ftableTwo) {
    double dist = 0.0;

    for (int i=0; i<BigramLanguageModel::TABLE_SIZE; ++i) {
        dist += (ftableOne[i] - ftableTwo[i]) * (ftableOne[i] - ftableTwo[i]);
    }

    return dist;
}

LatticeWorker* LangGuesser::doCreateLatticeWorker(Lattice& lattice) {
    return new Worker(*this, lattice);
}

std::string LangGuesser::doInfo() {
    return std::string("language guesser");
}

/*
 * LangGuesser::Factory
 */

Annotator* LangGuesser::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options) {

    LangGuesser *langGuesser = new LangGuesser(options);
    return langGuesser;
}

std::string LangGuesser::Factory::doGetName() const {
    return "lang-guesser";
}

boost::filesystem::path LangGuesser::Factory::doGetFile() const {
    return __FILE__;
}

std::list<std::list<std::string> > LangGuesser::Factory::doRequiredLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::list<std::string> > LangGuesser::Factory::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> LangGuesser::Factory::doProvidedLayerTags() {
    std::list<std::string> layerTags;
    //layerTags.push_back("guessed-lang");
    return layerTags;
}

boost::program_options::options_description LangGuesser::Factory::doOptionsHandled() {
    boost::program_options::options_description optionsDescription("Allowed options");

    optionsDescription.add_options()
        ("default-language", boost::program_options::value<std::string>()
         ->default_value(DEFAULT_DEFAULT_LANGUAGE),
         "Language code to be used for unrecognized strings, use 'none' to turn off putting a default language code")
        ("force", "All frags must be marked as text in some language")
        ("only-langs", boost::program_options::value<std::vector<std::string> >()->multitoken(),
         "Guesses language only from the given list of languages");


    return optionsDescription;
}

AnnotatorFactory::LanguagesHandling LangGuesser::Factory::doLanguagesHandling(
    const boost::program_options::variables_map& /*options*/) const {
    return LANGUAGE_GUESSER;
}

std::list<std::string> LangGuesser::Factory::doLanguagesHandled(
    const boost::program_options::variables_map& /*options*/) const {
    return std::list<std::string>();
}

/*
 * LangGuesser::Worker
 */

LangGuesser::Worker::Worker(LangGuesser& processor, Lattice& lattice):
    LatticeWorker(lattice),
    processor_(processor) {
    }

void LangGuesser::Worker::doRun() {
    guessLanguage_();
}

bool LangGuesser::Worker::guessLanguage_() {
    LayerTagMask textMask = lattice_.getLayerTagManager().getMask("frag");
    Lattice::EdgesSortedBySourceIterator edgeIter(lattice_, textMask);

    while (edgeIter.hasNext()) {
        Lattice::EdgeDescriptor edge = edgeIter.next();
        std::string text = lattice_.getEdgeAnnotationItem(edge).getText();

        std::string guessedLanguage = processor_.guessLanguage(text);
        INFO("Guessed language for text [" << text << "] is " << guessedLanguage);

        if (guessedLanguage != UNKNOWN_LANGUAGE)
            markLanguage_(guessedLanguage, edge);
    }

    return false;
}

void LangGuesser::Worker::markLanguage_(
    const std::string& language, Lattice::EdgeDescriptor edge) {

    AnnotationItem item("TEXT", lattice_.getEdgeAnnotationItem(edge).getText());

    Lattice::EdgeSequence::Builder edgeSequenceBuilder(lattice_);
    edgeSequenceBuilder.addEdge(edge);

    lattice_.addEdge(
        lattice_.getEdgeSource(edge),
        lattice_.getEdgeTarget(edge),
        item,
        getTagForLanguage_(language),
        edgeSequenceBuilder.build());
}

LayerTagCollection LangGuesser::Worker::getTagForLanguage_(const std::string& language) {
    return lattice_.getLayerTagManager().createTagCollectionFromList(
        boost::assign::list_of
        (LayerTagManager::getLanguageTag(language))
        ("text")
        ("lang-guesser"));
}
