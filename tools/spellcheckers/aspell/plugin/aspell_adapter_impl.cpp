#include "aspell_adapter_impl.hpp"

#include <boost/assign.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include "logging.hpp"

AspellAdapterImpl::AspellAdapterImpl() :
    limitCandidates_(0),
    langCode_("") {
    aspellSpeller_ = NULL;
    aspellConfig_ = new_aspell_config();
}


AspellAdapterImpl::~AspellAdapterImpl() {
    if (NULL != aspellSpeller_) {
        delete_aspell_speller(aspellSpeller_);
    }

    if (NULL != aspellConfig_) {
        delete_aspell_config(aspellConfig_);
    }
}

bool AspellAdapterImpl::isWordCorrect(const std::string & word) {
    int correct = aspell_speller_check(aspellSpeller_, word.c_str(), -1);

    return (bool) correct;
}

void AspellAdapterImpl::getSuggestionsForLastWord(
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

void AspellAdapterImpl::initAspell(const std::string & langCode) {
    langCode_ = langCode;
    aspell_config_replace(aspellConfig_, "lang", langCode.c_str());
    aspell_config_replace(aspellConfig_, "encoding", "utf-8");
}

void AspellAdapterImpl::passOptionsToAspellConfig(
                const boost::program_options::variables_map& options) {

    if (options.count("limit")) {
        limitCandidates_ = options["limit"].as<unsigned int>();
    } else {
        limitCandidates_ = 0;
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

void AspellAdapterImpl::createAspellInstance() {
    AspellCanHaveError * possibleError = new_aspell_speller(aspellConfig_);
    if (aspell_error_number(possibleError) != 0) {
        ERROR("ASPELL CREATION ERROR: " << aspell_error_message(possibleError));
        // @todo
        //        throw PsiException(aspell_error_message(possibleError));
    } else {
        aspellSpeller_ = to_aspell_speller(possibleError);
    }
}

bool AspellAdapterImpl::isDictionaryAvailable(const std::string & langCode) {
    AspellConfig * tempAspellConfig = new_aspell_config();
    aspell_config_replace(tempAspellConfig, "lang", langCode.c_str());
    AspellCanHaveError * possibleError = new_aspell_speller(tempAspellConfig);

    bool result = true;
    if (aspell_error_number(possibleError) != 0) {
        result = false;
    } else {
        AspellSpeller * tempSpeller = to_aspell_speller(possibleError);
        delete_aspell_speller(tempSpeller);
    }

    delete_aspell_config(tempAspellConfig);
    
    return result;
}

// ==============================================

extern "C" AspellAdapterImpl* create() {
    return new AspellAdapterImpl;
}

extern "C" void destroy(AspellAdapterImpl* Tl) {
    delete Tl ;
}
