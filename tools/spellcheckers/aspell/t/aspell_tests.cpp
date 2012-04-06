#include "tests.hpp"

#include "psi_aspell.hpp"

void checkIsSuggestionOccurs_(
                              PSIAspell & aspellAdapter,
                              const std::string & word,
                              const std::string & expectedSuggestion) {

    SuggestionsList suggestions;
    BOOST_CHECK(!aspellAdapter.isWordCorrect(word));
    aspellAdapter.getSuggestionsForLastWord(suggestions, word);
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
    PSIAspell aspellAdapter(std::string("pl_PL"));

    BOOST_CHECK(aspellAdapter.isWordCorrect("słowo"));
    BOOST_CHECK(!aspellAdapter.isWordCorrect("slowo"));

    checkIsSuggestionOccurs_(aspellAdapter, "slowo", "słowo");
    checkIsSuggestionOccurs_(aspellAdapter, "przep iękny", "przepiękny");
    checkIsSuggestionOccurs_(aspellAdapter, "londyn", "Londyn");
    checkIsSuggestionOccurs_(aspellAdapter, "niebiegnie", "nie biegnie");
}

BOOST_AUTO_TEST_CASE( run_aspell_english_simple ) {
    PSIAspell aspellAdapter(std::string("en_EN"));

    BOOST_CHECK(aspellAdapter.isWordCorrect("become"));
    BOOST_CHECK(!aspellAdapter.isWordCorrect("ocurence"));

    checkIsSuggestionOccurs_(aspellAdapter, "ocurence", "occurrence");
    checkIsSuggestionOccurs_(aspellAdapter, "bec oming", "becoming");
    checkIsSuggestionOccurs_(aspellAdapter, "london", "London");
    checkIsSuggestionOccurs_(aspellAdapter, "wassnowing", "was snowing");
}


BOOST_AUTO_TEST_SUITE_END()
