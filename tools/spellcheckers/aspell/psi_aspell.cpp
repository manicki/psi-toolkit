#include <list>
#include <boost/assign.hpp>
#include <boost/lexical_cast.hpp>

#include "psi_aspell.hpp"

#include "logging.hpp"
#include "config.hpp"
#include "psi_exception.hpp"

const unsigned int PSIAspell::Factory::DEFAULT_LIMIT = 5;
const std::list<std::string> PSIAspell::tagsToOperateOn = boost::assign::list_of("token");
const std::list<std::string> PSIAspell::tagsToPut = boost::assign::list_of
    ("token")("aspell")("corrected");

Annotator* PSIAspell::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options) {

    std::string lang = options["lang"].as<std::string>();

    return new PSIAspell(lang, options);
}

void PSIAspell::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription) {

    optionsDescription.add_options()
        ("limit",
         boost::program_options::value<unsigned int>()
         ->default_value(DEFAULT_LIMIT),
         "Take only limit candidates into account. If set to zero, then take all into account.")
        ("size",
         boost::program_options::value<std::string>(),
         "The preferred size of the word list. "
         "This consists of a two char digit code describing the size of "
         "the list, with typical values of: 10=tiny, 20=really small, 30=small, "
         "40=med-small, 50=med, 60=med-large, 70=large, 80=huge, 90=insane."
         )
        ("personal",
         boost::program_options::value<std::string>(),
         "Personal word list file name (proceed by ./ if you want to use current directory).")
        ("repl",
         boost::program_options::value<std::string>(),
         "Replacements list file name (proceed by ./ if you want to use current directory).")
        ("ignore",
         boost::program_options::value<int>(),
         "Ignore words with N characters or less.")
        ("keyboard",
         boost::program_options::value<std::string>(),
         "The base name of the keyboard definition file to use.")
        ("sug-mode",
         boost::program_options::value<std::string>(),
         "Suggestion mode = `ultra' | `fast' | `normal' | `slow' | `bad-spellers'.")
        ("ignore-case",
         "Ignore case when checking words.")
        ;
}

std::string PSIAspell::Factory::doGetName() const {
    return "aspell";
}

std::list<std::string> PSIAspell::Factory::doGetAliases() {
    return boost::assign::list_of<std::string>
        (std::string("spell"));
}

boost::filesystem::path PSIAspell::Factory::doGetFile() const {
    return __FILE__;
}

std::list<std::list<std::string> > PSIAspell::Factory::doRequiredLayerTags() {
    return boost::assign::list_of(boost::assign::list_of("token"));
}

std::list<std::list<std::string> > PSIAspell::Factory::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> PSIAspell::Factory::doProvidedLayerTags() {
    return PSIAspell::tagsToPut;
}

std::string PSIAspell::Factory::doGetContinuation(
    const boost::program_options::variables_map& /* options */) const {
    return "simple-writer";
}

std::list<std::string> PSIAspell::Factory::doAllLanguagesHandled() const {
    // taken from ftp://ftp.gnu.org/gnu/aspell/dict/0index.html
    return boost::assign::list_of
        (std::string("af"))
        (std::string("am"))
        (std::string("ar"))
        (std::string("ast"))
        (std::string("az"))
        (std::string("be"))
        (std::string("bg"))
        (std::string("bn"))
        (std::string("br"))
        (std::string("ca"))
        (std::string("cs"))
        (std::string("csb"))
        (std::string("cy"))
        (std::string("da"))
        (std::string("de"))
        (std::string("de-alt"))
        (std::string("el"))
        (std::string("en"))
        (std::string("eo"))
        (std::string("es"))
        (std::string("et"))
        (std::string("fa"))
        (std::string("fi"))
        (std::string("fo"))
        (std::string("fr"))
        (std::string("fy"))
        (std::string("ga"))
        (std::string("gd"))
        (std::string("gl"))
        (std::string("grc"))
        (std::string("gu"))
        (std::string("gv"))
        (std::string("he"))
        (std::string("hi"))
        (std::string("hil"))
        (std::string("hr"))
        (std::string("hsb"))
        (std::string("hu"))
        (std::string("hus"))
        (std::string("hy"))
        (std::string("ia"))
        (std::string("id"))
        (std::string("is"))
        (std::string("it"))
        (std::string("kn"))
        (std::string("ku"))
        (std::string("ky"))
        (std::string("la"))
        (std::string("lt"))
        (std::string("lv"))
        (std::string("mg"))
        (std::string("mi"))
        (std::string("mk"))
        (std::string("ml"))
        (std::string("mn"))
        (std::string("mr"))
        (std::string("ms"))
        (std::string("mt"))
        (std::string("nb"))
        (std::string("nds"))
        (std::string("nl"))
        (std::string("nn"))
        (std::string("ny"))
        (std::string("or"))
        (std::string("pa"))
        (std::string("pl"))
        (std::string("pt_BR"))
        (std::string("pt_PT"))
        (std::string("qu"))
        (std::string("ro"))
        (std::string("ru"))
        (std::string("rw"))
        (std::string("sc"))
        (std::string("sk"))
        (std::string("sl"))
        (std::string("sr"))
        (std::string("sv"))
        (std::string("sw"))
        (std::string("ta"))
        (std::string("te"))
        (std::string("tet"))
        (std::string("tk"))
        (std::string("tl"))
        (std::string("tn"))
        (std::string("tr"))
        (std::string("uk"))
        (std::string("uz"))
        (std::string("vi"))
        (std::string("wa"))
        (std::string("yi"))
        (std::string("zu"));
}

LatticeWorker* PSIAspell::doCreateLatticeWorker(Lattice& lattice) {
    return new Worker(*this, lattice);
}

PSIAspell::Worker::Worker(Processor& processor, Lattice& lattice):
    LatticeWorker(lattice), processor_(processor),
    textTags_(lattice_.getLayerTagManager().createTagCollectionFromList(
                                            PSIAspell::tagsToPut))
{
}

void PSIAspell::Worker::doRun() {

    LayerTagMask tokenMask_ =
          lattice_.getLayerTagManager().getMask(
                   PSIAspell::tagsToOperateOn);

    Lattice::EdgesSortedByTargetIterator edgeIterator
        = lattice_.edgesSortedByTarget(tokenMask_);

    Lattice::EdgeDescriptor lastTokenEdge;
    Lattice::EdgeDescriptor lastSeparatingEdge;
    bool wasLastSeparatingEdge = false;
    bool wasTokenEdgeInShortDistanceIncorrect = false;

    while (edgeIterator.hasNext()) {
        Lattice::EdgeDescriptor currentEdge = edgeIterator.next();
        std::string category = lattice_.getAnnotationCategory(currentEdge);

        if ("T" == category) {
            bool isCurrentTokenIncorrect = processCheckEdgeIsIncorrect_(currentEdge);

            if (wasTokenEdgeInShortDistanceIncorrect
                && isCurrentTokenIncorrect) {

                if (wasLastSeparatingEdge) {
                    processCheckMultiEdgesAreIncorrect_(lastTokenEdge,
                                            lastSeparatingEdge,
                                            currentEdge);
                } else {
                    processCheckMultiEdgesAreIncorrect_(lastTokenEdge,
                                            currentEdge);
                }
            }

            if (isCurrentTokenIncorrect) {
                wasLastSeparatingEdge = false;
                lastTokenEdge = currentEdge;
                wasTokenEdgeInShortDistanceIncorrect = true;
            } else {
                wasTokenEdgeInShortDistanceIncorrect = false;
            }

        } else if ("B" == category) {
            if (wasLastSeparatingEdge) {
                wasTokenEdgeInShortDistanceIncorrect = false;
            }

            lastSeparatingEdge = currentEdge;
            wasLastSeparatingEdge = true;
        } else {
            wasLastSeparatingEdge = false;
            wasTokenEdgeInShortDistanceIncorrect = false;
        }
    }
}

bool PSIAspell::Worker::processCheckEdgeIsIncorrect_(const Lattice::EdgeDescriptor & edgeToCheck) {
    std::string textToCheck = lattice_.getAnnotationText(edgeToCheck);
    return processAspellCheckOnText_(textToCheck,
                                     lattice_.getEdgeSource(edgeToCheck),
                                     lattice_.getEdgeTarget(edgeToCheck)
                                     );
}

bool PSIAspell::Worker::processCheckMultiEdgesAreIncorrect_(
                                       const Lattice::EdgeDescriptor & firstEdgeToCheck,
                                       const Lattice::EdgeDescriptor & separatingEdge,
                                       const Lattice::EdgeDescriptor & secondEdgeToCheck) {
    std::string textToCheck = lattice_.getAnnotationText(firstEdgeToCheck)
        + lattice_.getAnnotationText(separatingEdge)
        + lattice_.getAnnotationText(secondEdgeToCheck);

    return processAspellCheckOnText_(textToCheck,
                                     lattice_.getEdgeSource(firstEdgeToCheck),
                                     lattice_.getEdgeTarget(secondEdgeToCheck)
                                     );
}

bool PSIAspell::Worker::processCheckMultiEdgesAreIncorrect_(
                                       const Lattice::EdgeDescriptor & firstEdgeToCheck,
                                       const Lattice::EdgeDescriptor & secondEdgeToCheck) {
    std::string textToCheck = lattice_.getAnnotationText(firstEdgeToCheck)
        + lattice_.getAnnotationText(secondEdgeToCheck);

    return processAspellCheckOnText_(textToCheck,
                                     lattice_.getEdgeSource(firstEdgeToCheck),
                                     lattice_.getEdgeTarget(secondEdgeToCheck)
                                     );
}

bool PSIAspell::Worker::processAspellCheckOnText_(const std::string & textToCheck,
                                     const Lattice::VertexDescriptor & sourceVertex,
                                     const Lattice::VertexDescriptor & targetVertex) {
    SuggestionsList * suggestions = checkWordInAspell_(textToCheck);

    if (suggestions) {
        BOOST_FOREACH(std::string suggestion, *suggestions) {
            AnnotationItem annotationItem("T", suggestion);
            lattice_.addEdge(sourceVertex,
                             targetVertex,
                             annotationItem,
                             textTags_
                             );
        }

        delete suggestions;
        return true;
    } else {
        return false;
    }
}

SuggestionsList * PSIAspell::Worker::checkWordInAspell_(const std::string & text) {
    PSIAspell & aspellProcessor = dynamic_cast<PSIAspell&>(processor_);

    if (!aspellProcessor.isWordCorrect(text)) {
        SuggestionsList * suggestions = new SuggestionsList();
        aspellProcessor.getSuggestionsForLastWord(*suggestions, text);

        return suggestions;
    } else {
        return NULL;
    }
}

std::string PSIAspell::doInfo() {
    return "Aspell";
}

// ===================================
// PSIAspell

PSIAspell::PSIAspell(const std::string & langCode) :
    limitCandidates_(0)
{
    initPSIAspell_(langCode);
    createAspellInstance_();
}

PSIAspell::PSIAspell(const std::string & langCode,
                     const boost::program_options::variables_map& options) :
    limitCandidates_(0)
{
    initPSIAspell_(langCode);

    passOptionsToAspellConfig_(options);

    createAspellInstance_();
}

void PSIAspell::initPSIAspell_(const std::string & langCode) {
    langCode_ = langCode;
    aspellConfig_ = NULL;
    aspellSpeller_ = NULL;
    aspellConfig_ = new_aspell_config();
    aspell_config_replace(aspellConfig_, "lang", langCode.c_str());
    aspell_config_replace(aspellConfig_, "encoding", "utf-8");
}

void PSIAspell::passOptionsToAspellConfig_(
                const boost::program_options::variables_map& options) {

    if (options.count("limit")) {
        limitCandidates_ = options["limit"].as<unsigned int>();
    }

    std::list<std::string> stringOptions =
        boost::assign::list_of
        (std::string("size"))
        (std::string("personal"))
        (std::string("repl"))
        (std::string("keyboard"))
        (std::string("sug-mode"))
        ;

    BOOST_FOREACH (std::string & optionName, stringOptions) {
        if (options.count(optionName.c_str())) {
            std::string optionValue = options[optionName.c_str()].as<std::string>();
            aspell_config_replace(aspellConfig_, optionName.c_str(), optionValue.c_str());
        }
    }

    if (options.count("ignore-case")) {
        aspell_config_replace(aspellConfig_, "ignore-case", "true");
    }

    if (options.count("ignore")) {
        int ignoreLength = options["ignore"].as<int>();
        std::string ignoreLengthString = boost::lexical_cast<std::string>( ignoreLength);
        aspell_config_replace(aspellConfig_, "ignore", ignoreLengthString.c_str());
    }
}

void PSIAspell::createAspellInstance_() {
    AspellCanHaveError * possibleError = new_aspell_speller(aspellConfig_);
    if (aspell_error_number(possibleError) != 0) {
        ERROR("ASPELL CREATION ERROR: " << aspell_error_message(possibleError));
        throw PsiException(aspell_error_message(possibleError));
    } else {
        aspellSpeller_ = to_aspell_speller(possibleError);
    }
}

PSIAspell::~PSIAspell() {
    if (NULL != aspellSpeller_) {
        delete_aspell_speller(aspellSpeller_);
    }

    if (NULL != aspellConfig_) {
        delete_aspell_config(aspellConfig_);
    }
}

bool PSIAspell::isWordCorrect(const std::string & word) {
    int correct = aspell_speller_check(aspellSpeller_, word.c_str(), -1);

    return (bool) correct;
}

void PSIAspell::getSuggestionsForLastWord(
                       SuggestionsList & suggestionsList,
                       const std::string & word
                       ) {


    const AspellWordList * suggestions = aspell_speller_suggest(
                        aspellSpeller_,
                        word.c_str(), -1);

    AspellStringEnumeration * elements = aspell_word_list_elements(suggestions);

    const char * currentWordSuggestion;
    while ( (currentWordSuggestion = aspell_string_enumeration_next(elements)) != NULL ) {
        if (limitCandidates_ &&
            (limitCandidates_ <= suggestionsList.size())) {
            break;
        }

        suggestionsList.push_back(std::string(currentWordSuggestion));
    }

    delete_aspell_string_enumeration(elements);

    return;
}
