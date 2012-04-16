#include "tests.hpp"

#include "psi_aspell.hpp"

void checkIsSuggestionOccurs_(
                              PSIAspell & psiAspell,
                              const std::string & word,
                              const std::string & expectedSuggestion) {

    SuggestionsList suggestions;
    BOOST_CHECK(!psiAspell.getAdapter()->isWordCorrect(word));
    psiAspell.getAdapter()->getSuggestionsForLastWord(suggestions, word);
    bool hasSuggestion = false;

    BOOST_FOREACH(std::string suggestion, suggestions) {
        if (suggestion == expectedSuggestion) {
            hasSuggestion = true;
            break;
        }
    }
    
    BOOST_CHECK(hasSuggestion);
}

BOOST_AUTO_TEST_SUITE( aspell )

BOOST_AUTO_TEST_CASE( run_aspell_polish_simple ) {
    PSIAspell psiAspell(std::string("pl_PL"));

    BOOST_CHECK(psiAspell.getAdapter()->isWordCorrect("słowo"));
    BOOST_CHECK(psiAspell.getAdapter()->isWordCorrect("polska"));
    BOOST_CHECK(!psiAspell.getAdapter()->isWordCorrect("slowo"));

    checkIsSuggestionOccurs_(psiAspell, "slowo", "słowo");
    checkIsSuggestionOccurs_(psiAspell, "przep iękny", "przepiękny");
    checkIsSuggestionOccurs_(psiAspell, "londyn", "Londyn");
    checkIsSuggestionOccurs_(psiAspell, "niebiegnie", "nie biegnie");
}

BOOST_AUTO_TEST_CASE( run_aspell_english_simple ) {
    PSIAspell psiAspell(std::string("en_EN"));

    BOOST_CHECK(psiAspell.getAdapter()->isWordCorrect("become"));
    BOOST_CHECK(!psiAspell.getAdapter()->isWordCorrect("ocurence"));

    checkIsSuggestionOccurs_(psiAspell, "ocurence", "occurrence");
    checkIsSuggestionOccurs_(psiAspell, "bec oming", "becoming");
    checkIsSuggestionOccurs_(psiAspell, "london", "London");
    checkIsSuggestionOccurs_(psiAspell, "wassnowing", "was snowing");
}

BOOST_AUTO_TEST_SUITE_END()
