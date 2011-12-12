#include "lang_guesser.hpp"

#include "lattice.hpp"
#include "logging.hpp"

#define LOWER_LETTER(i) ( ((i) > 90 || (i) < 65) ? (i) : ((i) + 32) )

std::string LangGuesser::UNKNOWN = "unknown";

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
        return UNKNOWN;
    }

    static double textFrequencyTable[ModelLanguage::FTABLE_LENGTH];
    createFrequencyTable(text, textFrequencyTable);

    double dist;
    double minDist = 100000000000.0;
    std::string minLang = UNKNOWN;

    BOOST_FOREACH (ModelLanguage lang, languages_) {
        dist = distance(textFrequencyTable, lang.bigramFrequencyTable);
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

void LangGuesser::countBigrams(std::string text, int* ctable) {

    if (text.length() < 2) {
        return;
    }

    std::string::iterator iter = text.begin();
    std::string::iterator end = text.end();

    utf8::uint32_t a = utf8::next(iter, end);
    a = LOWER_LETTER(a);
    utf8::uint32_t b = utf8::next(iter, end);
    b = LOWER_LETTER(b);

    while (iter != end) {
        ++ctable[(a * BFACTOR + b * (BFACTOR * BFACTOR)) % ModelLanguage::FTABLE_LENGTH];
        a = b;
        b = utf8::next(iter, end);
        b = LOWER_LETTER(b);
    }
}

void LangGuesser::zeroCountTable(int* ctable) {
    for (int i = 0; i < ModelLanguage::FTABLE_LENGTH; i++) {
        ctable[i] = 0;
    }
}

void LangGuesser::zeroFrequencyTable(double* ftable) {
    for (int i = 0; i < ModelLanguage::FTABLE_LENGTH; i++) {
        ftable[i] = 0.0;
    }
}

void LangGuesser::bigramCountToFrequencyTable(int* ctable, double* ftable) {
    int sum = sumOfCounts(ctable);

    if (sum == 0) {
        zeroFrequencyTable(ftable);
    }
    else {
        for (int i = 0; i < ModelLanguage::FTABLE_LENGTH; i++) {
            ftable[i] = double(ctable[i]) / double(sum);
        }
    }
}

int LangGuesser::sumOfCounts(int* ctable) {
    int sum = 0;

    for (int i = 0; i < ModelLanguage::FTABLE_LENGTH; i++) {
        sum += ctable[i];
    }

    return sum;
}

double LangGuesser::distance(double* ftableOne, double* ftableTwo) {
    double dist = 0.0;

    for(int i=0; i<ModelLanguage::FTABLE_LENGTH; ++i) {
        dist += (ftableOne[i] - ftableTwo[i]) * (ftableOne[i] - ftableTwo[i]);
    }

    return dist;
}

void LangGuesser::createFrequencyTable(std::string text, double * ftable) {
    static int ctable[ModelLanguage::FTABLE_LENGTH];
    zeroCountTable(ctable);

    countBigrams(text, ctable);
    bigramCountToFrequencyTable(ctable, ftable);
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

