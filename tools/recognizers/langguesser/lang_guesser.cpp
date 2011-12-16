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

#define LOWER_LETTER(i) ( ((i) > 90 || (i) < 65) ? (i) : ((i) + 32) )

std::string LangGuesser::UNKNOWN_LANGUAGE = "unknown";

LangGuesser::LangGuesser() {
    initLanguages();
}

void LangGuesser::initLanguages() {

    ProcessorFileFetcher fileFetcher(__FILE__);

    languages_.push_back( Language("pl",
        fileFetcher.getOneFile("%ITSDATA%/pllang.i"),
        "ąćęłńóśźżĄĆĘŁŃÓŚŹŻ")
    );

    languages_.push_back( Language("en",
        fileFetcher.getOneFile("%ITSDATA%/enlang.i"),
        "")
    );

    languages_.push_back( Language("de",
        fileFetcher.getOneFile("%ITSDATA%/delang.i"),
        "äöüßÄÖÜ")
    );

    languages_.push_back( Language("ru",
        fileFetcher.getOneFile("%ITSDATA%/rulang.i"),
        "абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ")
    );
}

bool LangGuesser::guessLanguage(Lattice& lattice) {

    LayerTagMask textMask = lattice.getLayerTagManager().getMask("text");
    Lattice::EdgesSortedBySourceIterator edgeIter(lattice, textMask);

    while (edgeIter.hasNext()) {
        std::string text = lattice.getEdgeAnnotationItem(edgeIter.next()).getText();

        std::string guessedLanguage = (text.length() < MIN_TEXT_LENGTH_FOR_BIGRAM_METHOD) ?
            guessLanguageByLetters(text) : guessLanguage(text);

        INFO("Guessed language for text [" << text << "] is " << guessedLanguage);
    }

    return false;
}

std::string LangGuesser::guessLanguage(std::string text) {

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
    std::string selectedLanguage = UNKNOWN_LANGUAGE;

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
                    }
                    foundInWord = false;
                    isWord = false;
                }
                else {
                    isWord = true;
                }

                if (found && allWords) {
                    if (selectedLanguage == UNKNOWN_LANGUAGE) {
                        selectedLanguage = lang.name;
                    }
                    else {
                        return selectedLanguage;
                    }
                }
            }

        }
    }

    return selectedLanguage;
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

    LangGuesser *langGuesser = new LangGuesser();
    return langGuesser;
}

std::string LangGuesser::Factory::doGetName() {
    return "lang-guesser";
}

boost::filesystem::path LangGuesser::Factory::doGetFile() {
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

    return optionsDescription;
}

/*
 * LangGuesser::Worker
 */

LangGuesser::Worker::Worker(LangGuesser& processor, Lattice& lattice):
    LatticeWorker(lattice), processor_(processor) {
        //@todo: tu robi obiekt langGuesser
    }

void LangGuesser::Worker::doRun() {
    processor_.guessLanguage(lattice_);
}

