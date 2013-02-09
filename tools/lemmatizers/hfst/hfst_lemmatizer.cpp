#include "hfst_lemmatizer.hpp"
#include "language_dependent_annotator_factory.hpp"
#include "lang_specific_processor_file_fetcher.hpp"

HfstLemmatizer::HfstLemmatizer(const boost::program_options::variables_map& options)
    : level_(3), annotationManager_(NULL) {

    std::string lang = options["lang"].as<std::string>();
    setLanguage(lang);

    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);

    if (options.count("level") > 0) setLevel(options["level"].as<int>());

    init(options);
    std::string transducerFileSpec = options["fst"].as<std::string>();
    boost::filesystem::path transducerFile = fileFetcher.getOneFile(transducerFileSpec);
    setTransducerFile(transducerFile.string());

    if (isActive())
        initializeHfstTransducer();
}

HfstLemmatizer::~HfstLemmatizer() {
    if (hfstAdapter_)
        PluginManager::getInstance().destroyPluginAdapter("hfst", hfstAdapter_);
    if (hfstTagsParser_ != NULL)
        delete hfstTagsParser_;
    if (hfstStemmer_ != NULL)
        delete hfstStemmer_;
}

std::string HfstLemmatizer::getName() {
    return "hfst-lemmatizer";
}

boost::filesystem::path HfstLemmatizer::getFile() {
    return __FILE__;
}

std::list<std::string> HfstLemmatizer::getLayerTags() {
    std::list<std::string> layerTags;
    layerTags.push_back("hfst");
    layerTags.push_back("hfst-tagset");

    return layerTags;
}

std::string HfstLemmatizer::getLanguage() const {
    return language_;
}

AnnotatorFactory::LanguagesHandling HfstLemmatizer::languagesHandling(
    const boost::program_options::variables_map &options) {
    return LanguageDependentAnnotatorFactory::checkLangOption(options);
}

std::list<std::string> HfstLemmatizer::languagesHandled(
    const boost::program_options::variables_map& options) {
    if (LanguageDependentAnnotatorFactory::checkLangOption(options)
        == AnnotatorFactory::JUST_ONE_LANGUAGE)
        return boost::assign::list_of(options["lang"].as<std::string>());

    return boost::assign::list_of("de")("en")("fi")("fr")("it")("se");
}

bool HfstLemmatizer::checkRequirements(
    const boost::program_options::variables_map& options,
    std::ostream & message) {

    return PluginManager::getInstance().checkPluginRequirements(
        "hfst",
        options,
        message);
}


void HfstLemmatizer::setLevel(int lvl) {
    if (0 <= lvl && lvl <= 3) {
        level_ = lvl;
    }
}

void HfstLemmatizer::setLanguage(const std::string &lang) {
    language_ = lang;
}

void HfstLemmatizer::setTransducerFile(const std::string &fst) {
    transducerFile_ = fst;
}

void HfstLemmatizer::init(const boost::program_options::variables_map& options) {
    hfstAdapter_ = dynamic_cast<HfstAdapterInterface*>(
            PluginManager::getInstance().createPluginAdapter("hfst"));
    hfstTagsParser_ = initializeHfstTagsParser(options);
    hfstStemmer_ = initializeHfstStemmer(options);
}

bool HfstLemmatizer::isActive() {
    if (hfstAdapter_)
        return true;
    else
        return false;
}

void HfstLemmatizer::initializeHfstTransducer() {
    hfstAdapter_->initHfst(transducerFile_);
}

HfstTagsParser* HfstLemmatizer::initializeHfstTagsParser(const boost::program_options::variables_map &options) {
    if (language_ == "de")
        return dynamic_cast<HfstTagsParser*>(new HfstGermanTagsParser(options));
    if (language_ == "en")
        return dynamic_cast<HfstTagsParser*>(new HfstEnglishTagsParser(options));
    if (language_ == "fi")
        return dynamic_cast<HfstTagsParser*>(new HfstFinnishTagsParser(options));
    if (language_ == "fr")
        return dynamic_cast<HfstTagsParser*>(new HfstFrenchTagsParser(options));
    if (language_ == "it")
        return dynamic_cast<HfstTagsParser*>(new HfstItalianTagsParser(options));
    if (language_ == "se")
        return dynamic_cast<HfstTagsParser*>(new HfstSwedishTagsParser(options));
    return NULL;
}

HfstStemmer* HfstLemmatizer::initializeHfstStemmer(const boost::program_options::variables_map &options) {
    if (language_ == "de")
        return dynamic_cast<HfstStemmer*>(new HfstGermanStemmer(options));
    if (language_ == "en")
        return dynamic_cast<HfstStemmer*>(new HfstEnglishStemmer(options));
    if (language_ == "fi")
        return dynamic_cast<HfstStemmer*>(new HfstFinnishStemmer(options));
    if (language_ == "fr")
        return dynamic_cast<HfstStemmer*>(new HfstFrenchStemmer(options));
    if (language_ == "it")
        return dynamic_cast<HfstStemmer*>(new HfstItalianStemmer(options));
    if (language_ == "se")
        return dynamic_cast<HfstStemmer*>(new HfstSwedishStemmer(options));
    return NULL;
}


boost::program_options::options_description HfstLemmatizer::optionsHandled() {
    boost::program_options::options_description desc("Allowed options");

    LanguageDependentAnnotatorFactory::addLanguageDependentOptions(desc);

    desc.add_options()
        ("level", boost::program_options::value<int>()->default_value(3),
         "set word processing level 0-3")
        ("fst", boost::program_options::value<std::string>()
         ->default_value(DEFAULT_TRANSDUCER_FILE),
         "set morphological analyzer transducer file")
        ;

    boost::program_options::options_description enDesc("English-specific options");
    enDesc.add_options()
        ("include-derivatives", "include derivatives")
        ;

    boost::program_options::options_description fiDesc("Finnish-specific options");
    fiDesc.add_options()
        ("keep-original-attributes", "keep original HFST attribute tags")
        //@todo: split-pos-tags temporarily disabled as did not work as intended
        //("do-not-split-pos-tags", "do not split POS tags")
        ("include-compounds", "include compund words")
        ("include-enclitic-particles", "include enclitic particles (e.g. -kin, -han, -hän) as attributes of form edges")
        ("include-inflection-types", "include inflection type numbers as attributes of form edges")
        ;

    desc.add(enDesc);
    desc.add(fiDesc);

    return desc;
}

bool HfstLemmatizer::lemmatize(const std::string & word, AnnotationItemManager & manager,
        LemmatizerOutputIterator & iterator) {

    annotationManager_ = &manager;

    if (!isActive())
        return false;

    bool addedLemma = false;

    switch (level_) {
        case 0:
            break;
        case 1:
            addedLemma = stemsOnLemmaLevel_(word, iterator);
            break;
        case 2:
            addedLemma = stemsOnLexemeLevel_(word, iterator);
            break;
        default:
            addedLemma = stemsOnFormLevel_(word, iterator);
    }
    return addedLemma;
}

bool HfstLemmatizer::stemsOnLemmaLevel_(const std::string &word,
    LemmatizerOutputIterator & outputIterator) {
    HfstStems stems = hfstStemmer_->stem(word, *hfstAdapter_);

    bool addedLemma = false;
    std::set<std::string> lemmas = getLemmasFromStems(stems);

    for (std::set<std::string>::iterator lem = lemmas.begin(); 
            lem != lemmas.end(); ++lem) {
        outputIterator.addLemma(*lem);
        addedLemma = true;
    }
    return addedLemma;
}

bool HfstLemmatizer::stemsOnLexemeLevel_(const std::string &word,
    LemmatizerOutputIterator & outputIterator) {
    HfstStems stems = hfstStemmer_->stem(word, *hfstAdapter_);

    bool addedLemma = false;
    std::set<std::string> lemmas = getLemmasFromStems(stems);

    for (std::set<std::string>::iterator lem = lemmas.begin(); 
            lem != lemmas.end(); ++lem) {
        outputIterator.addLemma(*lem);
        addedLemma = true;

        HfstTags lexemeTags = getLexemeTagsFromStems(stems, *lem);

        for (HfstTags::iterator lxt = lexemeTags.begin(); 
                lxt != lexemeTags.end(); ++lxt) {
            AnnotationItem lexItem = createLexemeAnnotation(*lem, *lxt);
            outputIterator.addLexeme(lexItem);
        }
    }
    return addedLemma;
}

bool HfstLemmatizer::stemsOnFormLevel_(const std::string &word,
    LemmatizerOutputIterator & outputIterator) {
    HfstStems stems = hfstStemmer_->stem(word, *hfstAdapter_);

    bool addedLemma = false;
    std::set<std::string> lemmas = getLemmasFromStems(stems);

    for (std::set<std::string>::iterator lem = lemmas.begin(); 
            lem != lemmas.end(); ++lem) {
        outputIterator.addLemma(*lem);
        addedLemma = true;

        HfstTags lexemeTags = getLexemeTagsFromStems(stems, *lem);

        for (HfstTags::iterator lxt = lexemeTags.begin(); 
                lxt != lexemeTags.end(); ++lxt) {

            AnnotationItem lexItem = createLexemeAnnotation(*lem, *lxt);
            outputIterator.addLexeme(lexItem);

            HfstTags formTags = getFormTagsFromLexeme(stems, *lem, *lxt);

            for (HfstTags::iterator frms = formTags.begin(); 
                    frms != formTags.end(); ++frms) {

                AnnotationItem frmItm = createFormAnnotation(lexItem, *lem, *frms);
                outputIterator.addForm(frmItm);
            }
        }
    }
    return addedLemma;
}

std::set<std::string> HfstLemmatizer::getLemmasFromStems(
        HfstStems & stems) {

    std::set<std::string> lemmas;

    for (HfstStems::iterator s = stems.begin(); s != stems.end(); ++s) {
        lemmas.insert(lemmas.end(), s->first);
    }

    return lemmas;
}

HfstTags HfstLemmatizer::getLexemeTagsFromStems(HfstStems & stems, 
    const std::string & lemma) {
    HfstTags tags;

    for (HfstStems::iterator s = stems.begin(); s != stems.end(); ++s) {
        if (s->first == lemma) {
            HfstRawTags tagStrings = s->second;
            HfstTags lexemeAttributes = 
                hfstTagsParser_->getLexemeAttributes(tagStrings);
            tags.insert(tags.end(), lexemeAttributes.begin(), lexemeAttributes.end());
        }
    }
    HfstTags::iterator i = unique(tags.begin(), tags.end());
    tags.resize(i - tags.begin());
    return tags;
}

HfstTags HfstLemmatizer::getFormTagsFromLexeme(HfstStems & stems,
    const std::string & lemma,
    std::map<std::string, std::string> lexemeAttributes) {

    HfstTags tags;

    std::vector<std::string>::iterator t;

    for (HfstStems::iterator s = stems.begin(); s != stems.end(); ++s) {
        if (s->first == lemma) {
            HfstRawTags tagStrings = s->second;
            HfstTags formAttributes = 
                hfstTagsParser_->getFormAttributes(tagStrings);
            for (HfstTags::iterator formAttribIt = formAttributes.begin(); 
                formAttribIt != formAttributes.end(); ++ formAttribIt) {
                if (attributesMatch(*formAttribIt, lexemeAttributes)) {
                    tags.push_back(*formAttribIt);
                }
            }
        }
    }

    return tags;
}

bool HfstLemmatizer::attributesMatch(std::map<std::string, std::string> specificAttributes,
        std::map<std::string, std::string> generalAttributes) {
    for (std::map<std::string, std::string>::iterator genIt = generalAttributes.begin();
            genIt != generalAttributes.end(); ++ genIt) {
        for (std::map<std::string, std::string>::iterator specIt = specificAttributes.begin();
                specIt != specificAttributes.end(); ++ specIt) {
            if (genIt->first == specIt->first)
                if (genIt->second != specIt->second)
                    return false;
        }
    }
    return true;
}

AnnotationItem HfstLemmatizer::createLexemeAnnotation(
        const std::string& stem, std::map<std::string, std::string> attributes) {

    std::string partOfSpeech = attributes["pos"];
    std::string wordId = stem + LEMMA_CATEGORY_SEPARATOR + partOfSpeech;
    attributes.erase("pos");

    AnnotationItem lexeme(partOfSpeech, StringFrag(wordId));

    for (std::map<std::string, std::string>::iterator atr = 
            attributes.begin(); atr != attributes.end(); ++atr) {
        annotationManager_->setValue(lexeme, atr->first, atr->second);
    }

    return lexeme;
}

AnnotationItem HfstLemmatizer::createFormAnnotation(AnnotationItem & lexemeItem,
    const std::string& word, std::map<std::string, std::string> & attributes) {

        AnnotationItem formItem(lexemeItem, word);
        attributes.erase("pos");

        for (std::map<std::string, std::string>::iterator atr = 
                attributes.begin(); atr != attributes.end(); ++atr) {
            annotationManager_->setValue(formItem, atr->first, atr->second);
        }
        return formItem;
}

const std::string HfstLemmatizer::DEFAULT_TRANSDUCER_FILE
        = "%ITSDATA%/%LANG%.hfst.ol";
