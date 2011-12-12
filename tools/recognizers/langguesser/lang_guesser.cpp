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
        "ąćęłńóśźżĄĆĘŁŃÓŚŹŻ",
        fileFetcher.getOneFile("%ITSDATA%/pllang.i"))
    );

    languages_.push_back( Language("en",
        "",
        fileFetcher.getOneFile("%ITSDATA%/enlang.i"))
    );

    languages_.push_back( Language("de",
        "äöüßÄÖÜ",
        fileFetcher.getOneFile("%ITSDATA%/delang.i"))
    );

    languages_.push_back( Language("ru",
        "абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ",
        fileFetcher.getOneFile("%ITSDATA%/rulang.i"))
    );
}

bool LangGuesser::guessLanguage(Lattice& lattice) {
    std::string l = "";

    LayerTagMask textMask = lattice.getLayerTagManager().getMask("text");
    Lattice::EdgesSortedBySourceIterator edgeIter(lattice, textMask);

    while (edgeIter.hasNext()) {
        std::string txt = lattice.getEdgeAnnotationItem(edgeIter.next()).getText(),
        l = guessLanguage(txt);

        INFO("Guessed language for text [" << txt << "] is " << l);
    }

    return false;
}

std::string LangGuesser::guessLanguage(std::string text) {

    if (text.length() == 0) {
        return UNKNOWN_LANGUAGE;
    }

    BigramLanguageModel inputTextBigramModel(text);

    double dist;
    double minDist = 100000000000.0;
    std::string minLang = UNKNOWN_LANGUAGE;

    BOOST_FOREACH (Language lang, languages_) {
        dist = distance(inputTextBigramModel.frequencyTable(), lang.model.frequencyTable());
        DEBUG("LangGuesser info: " << lang.name << " => " << dist);

        if (dist < minDist) {
            minLang = lang.name;
            minDist = dist;
        }
    }

    return minLang;
}

std::string LangGuesser::guessLanguageByLetters(std::string text) {
    return std::string("unknown");
}

/*
 * Private methods
 */

double LangGuesser::distance(double* ftableOne, double* ftableTwo) {
    double dist = 0.0;

    for (int i=0; i<BigramLanguageModel::TABLE_SIZE; ++i) {
        dist += (ftableOne[i] - ftableTwo[i]) * (ftableOne[i] - ftableTwo[i]);
    }

    return dist;
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

