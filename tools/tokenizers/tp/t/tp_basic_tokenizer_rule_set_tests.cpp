#include "tests.hpp"

#include "tp_basic_tokenizer_rule_set.hpp"

#include "config.hpp"

BOOST_AUTO_TEST_SUITE( tp_basic_tokenizer_rule_set )

BOOST_AUTO_TEST_CASE( tokenize_simple ) {
    TPBasicTokenizerRuleSet ruleSet;

    ruleSet.load(ROOT_DIR "tools/tokenizers/tp/t/rules.rgx");

    size_t NB_REGEXPS = 6U;

    BOOST_REQUIRE_EQUAL(ruleSet.getRegexCount(), NB_REGEXPS);

    // the order of rules is reversed, that's why NB_REGEXP-i is used
    BOOST_CHECK_EQUAL(ruleSet.getRegexName(NB_REGEXPS-1), "text");
    BOOST_CHECK_EQUAL(ruleSet.getRegexCategory(NB_REGEXPS-1), "T");
    BOOST_CHECK_EQUAL(ruleSet.getRegex(NB_REGEXPS-1)->pattern(), "^([a-zA-Z]+)");

    BOOST_CHECK_EQUAL(ruleSet.getRegexName(NB_REGEXPS-2), "number");
    BOOST_CHECK_EQUAL(ruleSet.getRegexCategory(NB_REGEXPS-2), "T");
    BOOST_CHECK_EQUAL(ruleSet.getRegex(NB_REGEXPS-2)->pattern(), "^(\\d \\d{3})");

    BOOST_CHECK_EQUAL(ruleSet.getRegexName(NB_REGEXPS-3), "hashA");
    BOOST_CHECK_EQUAL(ruleSet.getRegexCategory(NB_REGEXPS-3), "T");
    BOOST_CHECK_EQUAL(ruleSet.getRegex(NB_REGEXPS-3)->pattern(), "^(#xyz)");

    BOOST_CHECK_EQUAL(ruleSet.getRegexName(NB_REGEXPS-4), "hashB");
    BOOST_CHECK_EQUAL(ruleSet.getRegexCategory(NB_REGEXPS-4), "T");
    BOOST_CHECK_EQUAL(ruleSet.getRegex(NB_REGEXPS-4)->pattern(), "^(#)");

    BOOST_CHECK_EQUAL(ruleSet.getRegexName(NB_REGEXPS-5), "space");
    BOOST_CHECK_EQUAL(ruleSet.getRegexCategory(NB_REGEXPS-5), "B");
    BOOST_CHECK_EQUAL(ruleSet.getRegex(NB_REGEXPS-5)->pattern(), "^( )");

    BOOST_CHECK_EQUAL(ruleSet.getRegexName(NB_REGEXPS-6), "polski");
    BOOST_CHECK_EQUAL(ruleSet.getRegexCategory(NB_REGEXPS-6), "T");
    BOOST_CHECK_EQUAL(ruleSet.getRegex(NB_REGEXPS-6)->pattern(), "^([a-zA-ZąćęłńóśźżĄĆĘŁŃÓŚŹŻ]+)");

}

BOOST_AUTO_TEST_CASE( loop1 ) {

    TPBasicTokenizerRuleSet ruleSet;

    BOOST_CHECK_THROW(
        ruleSet.load(ROOT_DIR "tools/tokenizers/tp/t/loop1.rgx"),
        TPTokenizerException);
}

BOOST_AUTO_TEST_CASE( loop2 ) {

    TPBasicTokenizerRuleSet ruleSet;

    BOOST_CHECK_THROW(
        ruleSet.load(ROOT_DIR "tools/tokenizers/tp/t/loop2.rgx"),
        TPTokenizerException);
}

BOOST_AUTO_TEST_CASE( loop3 ) {

    TPBasicTokenizerRuleSet ruleSet;

    BOOST_CHECK_THROW(
        ruleSet.load(ROOT_DIR "tools/tokenizers/tp/t/loop3.rgx"),
        TPTokenizerException);
}

BOOST_AUTO_TEST_CASE( include ) {

    TPBasicTokenizerRuleSet ruleSet;

    std::map<std::string, std::string> pathMap;
    pathMap["rest"] = ROOT_DIR "tools/tokenizers/tp/t/rest.rgx";
    pathMap["main"] = ROOT_DIR "tools/tokenizers/tp/t/main.rgx";
    pathMap["extra"] = ROOT_DIR "tools/tokenizers/tp/t/extra.rgx";

    std::list<std::string> paths;
    paths.push_back("main");

    ruleSet.load(pathMap, paths);

    size_t NB_REGEXPS = 4U;

    BOOST_REQUIRE_EQUAL(ruleSet.getRegexCount(), NB_REGEXPS);

    BOOST_CHECK_EQUAL(ruleSet.getRegexName(NB_REGEXPS-1), "text");
    BOOST_CHECK_EQUAL(ruleSet.getRegexCategory(NB_REGEXPS-1), "T");
    BOOST_CHECK_EQUAL(ruleSet.getRegex(NB_REGEXPS-1)->pattern(), "^([a-z]+)");

    BOOST_CHECK_EQUAL(ruleSet.getRegexName(NB_REGEXPS-2), "foo");
    BOOST_CHECK_EQUAL(ruleSet.getRegexCategory(NB_REGEXPS-2), "T");
    BOOST_CHECK_EQUAL(ruleSet.getRegex(NB_REGEXPS-2)->pattern(), "^(foo+)");

    BOOST_CHECK_EQUAL(ruleSet.getRegexName(NB_REGEXPS-3), "blank");
    BOOST_CHECK_EQUAL(ruleSet.getRegexCategory(NB_REGEXPS-3), "B");
    BOOST_CHECK_EQUAL(ruleSet.getRegex(NB_REGEXPS-3)->pattern(), "^(\\s)");

    BOOST_CHECK_EQUAL(ruleSet.getRegexName(NB_REGEXPS-4), "number");
    BOOST_CHECK_EQUAL(ruleSet.getRegexCategory(NB_REGEXPS-4), "T");
    BOOST_CHECK_EQUAL(ruleSet.getRegex(NB_REGEXPS-4)->pattern(), "^([0-9]+)");
}

BOOST_AUTO_TEST_CASE( tokenizer_hard ) {

    TPBasicTokenizerRuleSet ruleSet;

    std::map<std::string, std::string> pathMap;
    pathMap["main"] = ROOT_DIR "tools/tokenizers/tp/t/hard.rgx";
    pathMap["hardb"] = ROOT_DIR "tools/tokenizers/tp/t/hardb.rgx";
    pathMap["hardc"] = ROOT_DIR "tools/tokenizers/tp/t/hardc.rgx";

    std::list<std::string> paths;
    paths.push_back("main");

    ruleSet.load(pathMap, paths);

    size_t NB_REGEXPS = 3U;

    BOOST_REQUIRE_EQUAL(ruleSet.getRegexCount(), NB_REGEXPS);

    BOOST_CHECK_EQUAL(ruleSet.getRegexName(NB_REGEXPS-1), "mail");
    BOOST_CHECK_EQUAL(ruleSet.getRegexCategory(NB_REGEXPS-1), "T");
    BOOST_CHECK_EQUAL(ruleSet.getRegex(NB_REGEXPS-1)->pattern(), "^([a-z]+@[a-z]+\\.[a-z]+)");

    BOOST_CHECK_EQUAL(ruleSet.getRegexName(NB_REGEXPS-2), "baq");
    BOOST_CHECK_EQUAL(ruleSet.getRegexCategory(NB_REGEXPS-2), "T");
    BOOST_CHECK_EQUAL(ruleSet.getRegex(NB_REGEXPS-2)->pattern(), "^(c+)");

    BOOST_CHECK_EQUAL(ruleSet.getRegexName(NB_REGEXPS-3), "bla");
    BOOST_CHECK_EQUAL(ruleSet.getRegexCategory(NB_REGEXPS-3), "T");
    BOOST_CHECK_EQUAL(ruleSet.getRegex(NB_REGEXPS-3)->pattern(), "^(b+)");
}

BOOST_AUTO_TEST_CASE( heredoc ) {

    TPBasicTokenizerRuleSet ruleSet;

    ruleSet.load(ROOT_DIR "tools/tokenizers/tp/t/heredoc.rgx");

    size_t NB_REGEXPS = 2U;

    BOOST_REQUIRE_EQUAL(ruleSet.getRegexCount(), NB_REGEXPS);

    BOOST_CHECK_EQUAL(ruleSet.getRegexName(NB_REGEXPS-1), "hhh");
    BOOST_CHECK_EQUAL(ruleSet.getRegexCategory(NB_REGEXPS-1), "T");
    BOOST_CHECK_EQUAL(ruleSet.getRegex(NB_REGEXPS-1)->pattern(),
                      "^((?i:prof|e\\.g|Mr)\\.(?!\\p{L}))");

    BOOST_CHECK_EQUAL(ruleSet.getRegexName(NB_REGEXPS-2), "text");
    BOOST_CHECK_EQUAL(ruleSet.getRegexCategory(NB_REGEXPS-2), "T");
    BOOST_CHECK_EQUAL(ruleSet.getRegex(NB_REGEXPS-2)->pattern(),
                      "^(foo+)");

}

BOOST_AUTO_TEST_CASE( ifdef ) {

    TPBasicTokenizerRuleSet ruleSet;

    ruleSet.load(ROOT_DIR "tools/tokenizers/tp/t/ifdef.rgx");

    size_t NB_REGEXPS = 4U;

    BOOST_REQUIRE_EQUAL(ruleSet.getRegexCount(), NB_REGEXPS);

    BOOST_CHECK_EQUAL(ruleSet.getRegexName(NB_REGEXPS-1), "text");
    BOOST_CHECK_EQUAL(ruleSet.getRegexCategory(NB_REGEXPS-1), "T");
    BOOST_CHECK_EQUAL(ruleSet.getRegex(NB_REGEXPS-1)->pattern(),
                      "^([a-z]+)");

    BOOST_CHECK_EQUAL(ruleSet.getRegexName(NB_REGEXPS-2), "number");
    BOOST_CHECK_EQUAL(ruleSet.getRegexCategory(NB_REGEXPS-2), "T");
    BOOST_CHECK_EQUAL(ruleSet.getRegex(NB_REGEXPS-2)->pattern(),
                      "^([0-9]+)");

    BOOST_CHECK_EQUAL(ruleSet.getRegexName(NB_REGEXPS-3), "blank");
    BOOST_CHECK_EQUAL(ruleSet.getRegexCategory(NB_REGEXPS-3), "B");
    BOOST_CHECK_EQUAL(ruleSet.getRegex(NB_REGEXPS-3)->pattern(),
                      "^(\\s)");

    BOOST_CHECK_EQUAL(ruleSet.getRegexName(NB_REGEXPS-4), "bla");
    BOOST_CHECK_EQUAL(ruleSet.getRegexCategory(NB_REGEXPS-4), "T");
    BOOST_CHECK_EQUAL(ruleSet.getRegex(NB_REGEXPS-4)->pattern(),
                      "^(bla+)");

}

BOOST_AUTO_TEST_SUITE_END()
