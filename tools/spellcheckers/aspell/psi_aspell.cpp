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

bool PSIAspell::Factory::doCheckRequirements(
                     const boost::program_options::variables_map& options,
                     std::ostream & message) const {
    return PluginManager::getInstance().checkPluginRequirements("aspell",
                                                                options,
                                                                message);
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
    PSIAspell & aspellProcessor = dynamic_cast<PSIAspell&>(processor_);

    if (aspellProcessor.isActive()) {

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

    if (!aspellProcessor.getAdapter()->isWordCorrect(text)) {
        SuggestionsList * suggestions = new SuggestionsList();
        aspellProcessor.getAdapter()->getSuggestionsForLastWord(*suggestions, text);

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

PSIAspell::PSIAspell(const std::string & langCode) {
    init_();

    if ( isActive() ) {
        aspellAdapter_->initAspell(langCode);
        aspellAdapter_->createAspellInstance();
    }
}

PSIAspell::PSIAspell(const std::string & langCode,
                     const boost::program_options::variables_map& options) {

    init_();

    if ( isActive() ) {
        aspellAdapter_->initAspell(langCode);
        aspellAdapter_->passOptionsToAspellConfig(options);
        aspellAdapter_->createAspellInstance();
    }
}

PSIAspell::~PSIAspell() {
    if (aspellAdapter_) {
        PluginManager::getInstance().destroyPluginAdapter("aspell", aspellAdapter_);
    }
}

void PSIAspell::init_() {
    aspellAdapter_ = dynamic_cast<AspellAdapterInterface*>(
                     PluginManager::getInstance().createPluginAdapter("aspell"));
}

AspellAdapterInterface * PSIAspell::getAdapter() {
    return aspellAdapter_;
}

bool PSIAspell::isActive() {
    if (aspellAdapter_) {
        return true;
    } else {
        return false;
    }
}
