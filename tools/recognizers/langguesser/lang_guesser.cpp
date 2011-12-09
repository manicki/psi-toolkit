#include "lang_guesser.hpp"

LangGuesser::LangGuesser() {
    initLanguages();
}

void LangGuesser::initLanguages() {
    languages_.push_back(ModelLanguage("pl", "pllang.i", "ąćęłńóśźżĄĆĘŁŃÓŚŹŻ"));
    languages_.push_back(ModelLanguage("en", "enlang.i", ""));
    languages_.push_back(ModelLanguage("de", "delang.i", "äöüßÄÖÜ"));
    languages_.push_back(ModelLanguage("ru", "rulang.i",
        "абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"));
}

bool LangGuesser::guessLanguage(Lattice& lattice) {
    return false;
}

std::string LangGuesser::guessLanguage(std::string text) {
    return std::string("unknown");
}

std::string LangGuesser::guessLanguageByLetters(std::string text) {
    return std::string("unknown");
}

/*
 * Implemented virtual methods
 */

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

