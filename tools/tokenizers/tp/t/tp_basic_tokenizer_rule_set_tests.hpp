#include "../tp_basic_tokenizer_rule_set.hpp"

#include "config.h"

class TpBasicTokenizerRuleSetTests : public CxxTest::TestSuite {

public:

    void testSimple() {
        TPBasicTokenizerRuleSet ruleSet;

        ruleSet.load(ROOT_DIR "tools/tokenizers/tp/t/rules.rgx");

        size_t NB_REGEXPS = 6U;

        TS_ASSERT_EQUALS(ruleSet.getRegexCount(), NB_REGEXPS);

        // the order of rules is reversed, that's why NB_REGEXP-i is used
        TS_ASSERT_EQUALS(ruleSet.getRegexName(NB_REGEXPS-1), "text");
        TS_ASSERT_EQUALS(ruleSet.getRegexCategory(NB_REGEXPS-1), "T");
        TS_ASSERT_EQUALS(ruleSet.getRegex(NB_REGEXPS-1)->pattern(), "^([a-zA-Z]+)");

        TS_ASSERT_EQUALS(ruleSet.getRegexName(NB_REGEXPS-2), "number");
        TS_ASSERT_EQUALS(ruleSet.getRegexCategory(NB_REGEXPS-2), "T");
        TS_ASSERT_EQUALS(ruleSet.getRegex(NB_REGEXPS-2)->pattern(), "^(\\d \\d{3})");

        TS_ASSERT_EQUALS(ruleSet.getRegexName(NB_REGEXPS-3), "hashA");
        TS_ASSERT_EQUALS(ruleSet.getRegexCategory(NB_REGEXPS-3), "T");
        TS_ASSERT_EQUALS(ruleSet.getRegex(NB_REGEXPS-3)->pattern(), "^(#xyz)");

        TS_ASSERT_EQUALS(ruleSet.getRegexName(NB_REGEXPS-4), "hashB");
        TS_ASSERT_EQUALS(ruleSet.getRegexCategory(NB_REGEXPS-4), "T");
        TS_ASSERT_EQUALS(ruleSet.getRegex(NB_REGEXPS-4)->pattern(), "^(#)");

        TS_ASSERT_EQUALS(ruleSet.getRegexName(NB_REGEXPS-5), "space");
        TS_ASSERT_EQUALS(ruleSet.getRegexCategory(NB_REGEXPS-5), "B");
        TS_ASSERT_EQUALS(ruleSet.getRegex(NB_REGEXPS-5)->pattern(), "^( )");

        TS_ASSERT_EQUALS(ruleSet.getRegexName(NB_REGEXPS-6), "polski");
        TS_ASSERT_EQUALS(ruleSet.getRegexCategory(NB_REGEXPS-6), "T");
        TS_ASSERT_EQUALS(ruleSet.getRegex(NB_REGEXPS-6)->pattern(), "^([a-zA-ZąćęłńóśźżĄĆĘŁŃÓŚŹŻ]+)");

    }

    void testLoop1() {
        TPBasicTokenizerRuleSet ruleSet;

        TS_ASSERT_THROWS(
            ruleSet.load(ROOT_DIR "tools/tokenizers/tp/t/loop1.rgx"),
            TPTokenizerException);
    }

    void testLoop2() {
        TPBasicTokenizerRuleSet ruleSet;

        TS_ASSERT_THROWS(
            ruleSet.load(ROOT_DIR "tools/tokenizers/tp/t/loop2.rgx"),
            TPTokenizerException);
    }

    void testLoop3() {
        TPBasicTokenizerRuleSet ruleSet;

        TS_ASSERT_THROWS(
            ruleSet.load(ROOT_DIR "tools/tokenizers/tp/t/loop3.rgx"),
            TPTokenizerException);
    }

    void testInclude() {
        TPBasicTokenizerRuleSet ruleSet;

        std::map<std::string, std::string> pathMap;
        pathMap["rest"] = ROOT_DIR "tools/tokenizers/tp/t/rest.rgx";
        pathMap["main"] = ROOT_DIR "tools/tokenizers/tp/t/main.rgx";
        pathMap["extra"] = ROOT_DIR "tools/tokenizers/tp/t/extra.rgx";

        std::list<std::string> paths;
        paths.push_back("main");

        ruleSet.load(pathMap, paths);

        size_t NB_REGEXPS = 4U;

        TS_ASSERT_EQUALS(ruleSet.getRegexCount(), NB_REGEXPS);

        TS_ASSERT_EQUALS(ruleSet.getRegexName(NB_REGEXPS-1), "text");
        TS_ASSERT_EQUALS(ruleSet.getRegexCategory(NB_REGEXPS-1), "T");
        TS_ASSERT_EQUALS(ruleSet.getRegex(NB_REGEXPS-1)->pattern(), "^([a-z]+)");

        TS_ASSERT_EQUALS(ruleSet.getRegexName(NB_REGEXPS-2), "foo");
        TS_ASSERT_EQUALS(ruleSet.getRegexCategory(NB_REGEXPS-2), "T");
        TS_ASSERT_EQUALS(ruleSet.getRegex(NB_REGEXPS-2)->pattern(), "^(foo+)");

        TS_ASSERT_EQUALS(ruleSet.getRegexName(NB_REGEXPS-3), "blank");
        TS_ASSERT_EQUALS(ruleSet.getRegexCategory(NB_REGEXPS-3), "B");
        TS_ASSERT_EQUALS(ruleSet.getRegex(NB_REGEXPS-3)->pattern(), "^(\\s)");

        TS_ASSERT_EQUALS(ruleSet.getRegexName(NB_REGEXPS-4), "number");
        TS_ASSERT_EQUALS(ruleSet.getRegexCategory(NB_REGEXPS-4), "T");
        TS_ASSERT_EQUALS(ruleSet.getRegex(NB_REGEXPS-4)->pattern(), "^([0-9]+)");
    }

    void testHard() {
        TPBasicTokenizerRuleSet ruleSet;

        std::map<std::string, std::string> pathMap;
        pathMap["main"] = ROOT_DIR "tools/tokenizers/tp/t/hard.rgx";
        pathMap["hardb"] = ROOT_DIR "tools/tokenizers/tp/t/hardb.rgx";
        pathMap["hardc"] = ROOT_DIR "tools/tokenizers/tp/t/hardc.rgx";

        std::list<std::string> paths;
        paths.push_back("main");

        ruleSet.load(pathMap, paths);

        size_t NB_REGEXPS = 3U;

        TS_ASSERT_EQUALS(ruleSet.getRegexCount(), NB_REGEXPS);

        TS_ASSERT_EQUALS(ruleSet.getRegexName(NB_REGEXPS-1), "mail");
        TS_ASSERT_EQUALS(ruleSet.getRegexCategory(NB_REGEXPS-1), "T");
        TS_ASSERT_EQUALS(ruleSet.getRegex(NB_REGEXPS-1)->pattern(), "^([a-z]+@[a-z]+\\.[a-z]+)");

        TS_ASSERT_EQUALS(ruleSet.getRegexName(NB_REGEXPS-2), "baq");
        TS_ASSERT_EQUALS(ruleSet.getRegexCategory(NB_REGEXPS-2), "T");
        TS_ASSERT_EQUALS(ruleSet.getRegex(NB_REGEXPS-2)->pattern(), "^(c+)");

        TS_ASSERT_EQUALS(ruleSet.getRegexName(NB_REGEXPS-3), "bla");
        TS_ASSERT_EQUALS(ruleSet.getRegexCategory(NB_REGEXPS-3), "T");
        TS_ASSERT_EQUALS(ruleSet.getRegex(NB_REGEXPS-3)->pattern(), "^(b+)");
    }

    void testHereDoc() {
        TPBasicTokenizerRuleSet ruleSet;

        ruleSet.load(ROOT_DIR "tools/tokenizers/tp/t/heredoc.rgx");

        size_t NB_REGEXPS = 2U;

        TS_ASSERT_EQUALS(ruleSet.getRegexCount(), NB_REGEXPS);

        TS_ASSERT_EQUALS(ruleSet.getRegexName(NB_REGEXPS-1), "hhh");
        TS_ASSERT_EQUALS(ruleSet.getRegexCategory(NB_REGEXPS-1), "T");
        TS_ASSERT_EQUALS(ruleSet.getRegex(NB_REGEXPS-1)->pattern(),
                         "^((?i:prof|e\\.g|Mr)\\.(?!\\p{L}))");

        TS_ASSERT_EQUALS(ruleSet.getRegexName(NB_REGEXPS-2), "text");
        TS_ASSERT_EQUALS(ruleSet.getRegexCategory(NB_REGEXPS-2), "T");
        TS_ASSERT_EQUALS(ruleSet.getRegex(NB_REGEXPS-2)->pattern(),
                         "^(foo+)");

    }

    void testIfDef() {
        TPBasicTokenizerRuleSet ruleSet;

        ruleSet.load(ROOT_DIR "tools/tokenizers/tp/t/ifdef.rgx");

        size_t NB_REGEXPS = 4U;

        TS_ASSERT_EQUALS(ruleSet.getRegexCount(), NB_REGEXPS);

        TS_ASSERT_EQUALS(ruleSet.getRegexName(NB_REGEXPS-1), "text");
        TS_ASSERT_EQUALS(ruleSet.getRegexCategory(NB_REGEXPS-1), "T");
        TS_ASSERT_EQUALS(ruleSet.getRegex(NB_REGEXPS-1)->pattern(),
                         "^([a-z]+)");

        TS_ASSERT_EQUALS(ruleSet.getRegexName(NB_REGEXPS-2), "number");
        TS_ASSERT_EQUALS(ruleSet.getRegexCategory(NB_REGEXPS-2), "T");
        TS_ASSERT_EQUALS(ruleSet.getRegex(NB_REGEXPS-2)->pattern(),
                         "^([0-9]+)");

        TS_ASSERT_EQUALS(ruleSet.getRegexName(NB_REGEXPS-3), "blank");
        TS_ASSERT_EQUALS(ruleSet.getRegexCategory(NB_REGEXPS-3), "B");
        TS_ASSERT_EQUALS(ruleSet.getRegex(NB_REGEXPS-3)->pattern(),
                         "^(\\s)");

        TS_ASSERT_EQUALS(ruleSet.getRegexName(NB_REGEXPS-4), "bla");
        TS_ASSERT_EQUALS(ruleSet.getRegexCategory(NB_REGEXPS-4), "T");
        TS_ASSERT_EQUALS(ruleSet.getRegex(NB_REGEXPS-4)->pattern(),
                         "^(bla+)");

    }

};
