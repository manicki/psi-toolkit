#include "tests.hpp"

#include "lang_guesser.hpp"

BOOST_AUTO_TEST_SUITE( lang_guesser )

BOOST_AUTO_TEST_CASE( long_text ) {
    LangGuesser langGuesser;

    std::string pl = "Jest to dwudziesta trzecia litera alfabetu greckiego. W greckim systemie "
        "liczbowym oznacza liczbę 700";
    std::string en = "It is the 23rd letter of the Greek alphabet and has a numeric value of 700.";
    std::string de = "Es ist der 23. Buchstabe des griechischen Alphabets und hat nach dem "
        "milesischen Prinzip einen numerischen Wert von 700";
    std::string ru = "23-я буква греческого алфавита. В системе греческой алфавитной записи "
        "чисел имеет числовое значение 700";

    BOOST_CHECK_EQUAL(langGuesser.guessLanguage(pl), "pl");
    BOOST_CHECK_EQUAL(langGuesser.guessLanguage(en), "en");
    BOOST_CHECK_EQUAL(langGuesser.guessLanguage(de), "de");
    BOOST_CHECK_EQUAL(langGuesser.guessLanguage(ru), "ru");

}

BOOST_AUTO_TEST_CASE( short_text ) {
    LangGuesser langGuesser;

    std::string lang = langGuesser.guessLanguageByLetters("Alę miało kocię");
    BOOST_CHECK_EQUAL(lang, "pl");
    lang = langGuesser.guessLanguageByLetters(" Alę miało kocię");
    BOOST_CHECK_EQUAL(lang, "pl");
    lang = langGuesser.guessLanguageByLetters("Alę miało kocię. ");
    BOOST_CHECK_EQUAL(lang, "pl");

    lang = langGuesser.guessLanguageByLetters(std::string("Ala ma kota"));
    BOOST_CHECK_EQUAL(lang, "unknown");

    lang = langGuesser.guessLanguageByLetters(std::string("Alę miał kot"));
    BOOST_CHECK_EQUAL(lang, "unknown");
    lang = langGuesser.guessLanguageByLetters(std::string("Ala miała kocię"));
    BOOST_CHECK_EQUAL(lang, "unknown");
    lang = langGuesser.guessLanguageByLetters(std::string("Alę ma kocię"));
    BOOST_CHECK_EQUAL(lang, "unknown");

    lang = langGuesser.guessLanguageByLetters(" älę miäl köt");
    BOOST_CHECK_EQUAL(lang, "de");
}

BOOST_AUTO_TEST_SUITE_END()
