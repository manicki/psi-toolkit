#include <list>
#include <boost/assign.hpp>

#include "psi_aspell.hpp"

#include "logging.hpp"
#include "config.hpp"

Annotator* PSIAspell::Factory::doCreateAnnotator(
    const boost::program_options::variables_map& options) {

    std::string lang = options["lang"].as<std::string>();

    // @todo
    return new PSIAspell(lang);
}

void PSIAspell::Factory::doAddLanguageIndependentOptionsHandled(
    boost::program_options::options_description& optionsDescription) {

    // @todo

}

std::string PSIAspell::Factory::doGetName() {
    return "aspell";
}

boost::filesystem::path PSIAspell::Factory::doGetFile() {
    return __FILE__;
}

std::list<std::list<std::string> > PSIAspell::Factory::doRequiredLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::list<std::string> > PSIAspell::Factory::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> PSIAspell::Factory::doProvidedLayerTags() {
    std::list<std::string> layerTags;
    layerTags.push_back("token");
    return layerTags;
}

std::string PSIAspell::Factory::doGetContinuation(
    const boost::program_options::variables_map& /* options */) const {
    return "simple-writer";
}

LatticeWorker* PSIAspell::doCreateLatticeWorker(Lattice& lattice) {
    return new Worker(*this, lattice);
}

PSIAspell::Worker::Worker(Processor& processor, Lattice& lattice):
    LatticeWorker(lattice), processor_(processor){
}

void PSIAspell::Worker::doRun() {
    DEBUG("starting aspell...");

    DEBUG("PROCESSOR INFO: " << processor_.doInfo());
    PSIAspell& aspellProcessor = dynamic_cast<PSIAspell&>(processor_);

    std::list<std::string> tagsToOperateOn_ =
        boost::assign::list_of(std::string("token"));

    LayerTagMask tokenMask_ =
          lattice_.getLayerTagManager().getMask(
                         tagsToOperateOn_);

    Lattice::EdgesSortedByTargetIterator edgeIterator
        = lattice_.edgesSortedByTarget(tokenMask_);

    while (edgeIterator.hasNext()) {
        Lattice::EdgeDescriptor edge = edgeIterator.next();
        std::string edgeText = lattice_.getAnnotationText(edge);
        std::string category = lattice_.getAnnotationCategory(edge);

        if ("T" == category) {
            DEBUG("PROCESSING ASPELL: " << edgeText);
            if (!aspellProcessor.isWordCorrect(edgeText)) {
                DEBUG("INCORRECT WORD: " << edgeText);

                std::list<std::string> suggestions;
                aspellProcessor.getSuggestionsForLastWord(suggestions, edgeText);

                if (!suggestions.empty()) {
                    BOOST_FOREACH(std::string& suggestion, suggestions) {
                        DEBUG("\t" << suggestion);
                    }
                } else {
                    DEBUG("No suggestions...");
                }
            }
        }
    }
}

std::string PSIAspell::doInfo() {
    return "Aspell";
}

// ===================================
// PSIAspell

PSIAspell::PSIAspell(const std::string & langCode):
    langCode_(langCode),
    aspellConfig_(NULL),
    aspellSpeller_(NULL)
{

    aspellConfig_ = new_aspell_config();
    aspell_config_replace(aspellConfig_, "lang", langCode.c_str());

    AspellCanHaveError * possibleError = new_aspell_speller(aspellConfig_);
    if (aspell_error_number(possibleError) != 0) {
        // @todo
        ERROR("ASPELL CREATION ERROR: " << aspell_error_message(possibleError));
    } else {
        aspellSpeller_ = to_aspell_speller(possibleError);
    }
    
}

PSIAspell::~PSIAspell() {
    if (aspellSpeller_) {
        delete aspellSpeller_;
    }

    if (aspellConfig_) {
        delete aspellConfig_;
    }
}

bool PSIAspell::isWordCorrect(const std::string & word) {
    // @todo
    int correct = aspell_speller_check(aspellSpeller_, word.c_str(), -1);

    return (bool) correct;
}

void PSIAspell::getSuggestionsForLastWord(
                       std::list<std::string> & suggestionsList,
                       const std::string & word
                       ) {

    /**
    AspellWordList * suggestions = aspell_speller_suggest(
                        aspellSpeller_,
                        word.c_str(), -1);

    AspellStringEnumeration * elements = aspell_word_list_elements(suggestions);

    char * word;
    while ( (word = aspell_string_enumeration_next(elements)) != NULL ) {
        suggestionsList.push_back(std::string(word));
    }
    delete_aspell_string_enumeration(elements);
    */
    return;
}
