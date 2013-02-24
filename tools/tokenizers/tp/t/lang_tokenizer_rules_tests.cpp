#include "tests.hpp"

#include "lattice.hpp"
#include "annotation_item_manager.hpp"
#include "tp_token_cutter.hpp"
#include "tp_basic_tokenizer_rule_set.hpp"
#include <boost/assign.hpp>

BOOST_AUTO_TEST_SUITE( tp_tokenizer_rules )

void initTestLattice_(Lattice &lattice, const std::string &text,
        const std::string &lang);
void test_tokenizer(const std::string &lang, const std::string &sentence,
        std::list<std::string> expected);

BOOST_AUTO_TEST_CASE( finnish_tokenizer_rules ) {
    std::list<std::string> expectedTokens1 = boost::assign::list_of
        ("Pidän")
        ("mm.")
        ("appelsiinista")
        (",")
        ("omenasta")
        ("ja")
        ("makkarasta")
        (".");
    test_tokenizer("fi", "Pidän mm. appelsiinista, omenasta ja makkarasta.", expectedTokens1);
    std::list<std::string> expectedTokens2 = boost::assign::list_of
        ("Täällä")
        ("on")
        ("kirahvia")
        (",")
        ("norsuja")
        (",")
        ("virtahepoja")
        ("jne.");
    test_tokenizer("fi", "Täällä on kirahvia, norsuja, virtahepoja jne.", expectedTokens2);
}

BOOST_AUTO_TEST_CASE( swedish_tokenizer_rules ) {
    std::list<std::string> expectedTokens1 = boost::assign::list_of
        ("I")
        ("Polen")
        ("bor")
        ("ca.")
        ("38")
        ("miljoner")
        ("människor")
        (".");
    test_tokenizer("se", "I Polen bor ca. 38 miljoner människor.", expectedTokens1);
    std::list<std::string> expectedTokens2 = boost::assign::list_of
        ("I")
        ("Stockholm")
        ("kan")
        ("man")
        ("t.ex.")
        ("äta")
        ("köttbullar")
        (".");
    test_tokenizer("se", "I Stockholm kan man t.ex. äta köttbullar.", expectedTokens2);
}

void initTestLattice_(Lattice &lattice, const std::string &text,
        const std::string &lang) {
    lattice.appendStringWithSymbols(text);
    LayerTagCollection textTags(
            lattice.getLayerTagManager().
            createSingletonTagCollectionWithLangCode("text", lang));
    AnnotationItem item("TEXT", text);
    try {
        lattice.addEdge(lattice.getFirstVertex(), lattice.getLastVertex(), item, textTags);
    } catch (LoopEdgeException) { }
}

void test_tokenizer(const std::string &lang, const std::string &sentence,
        std::list<std::string> expected) {
    AnnotationItemManager aim;
    Lattice lattice(aim);
    initTestLattice_(lattice, sentence, lang);

    boost::scoped_ptr<TPBasicTokenizerRuleSet> ruleSet(new TPBasicTokenizerRuleSet());
    std::map<std::string, std::string> pathMap;
    pathMap["common"] = ROOT_DIR "tools/tokenizers/tp/data/common.rgx";
    pathMap["abbrev_" + lang] = ROOT_DIR "tools/tokenizers/tp/data/" + lang + "/abbrev.rgx";
    pathMap["main"] = ROOT_DIR "tools/tokenizers/tp/data/" + lang + "/" + lang + ".rgx";

    std::list<std::string> paths;
    paths.push_back("main");
    ruleSet->load(pathMap, paths);

    TpTokenCutter tokenCutter(
                lang,
                *ruleSet.get(),
                1000,
                995);

    LayerTagMask symbolMask = lattice.getLayerTagManager().getMask("symbol");
    LayerTagMask textMask = lattice.getLayerTagManager().getMaskWithLangCode(
            "text", lang);

    lattice.runCutter(tokenCutter, symbolMask, textMask);

    std::list<std::string> resultTokens;
    LayerTagMask tokenMask = lattice.getLayerTagManager().getMask("token");
    Lattice::EdgesSortedByTargetIterator tokenIter = lattice.edgesSortedByTarget(tokenMask);
    while (tokenIter.hasNext()) {
        Lattice::EdgeDescriptor edge = tokenIter.next();
        AnnotationItem annotationItem = lattice.getEdgeAnnotationItem(edge);
        if (annotationItem.getCategory() != "B")
            resultTokens.push_back(annotationItem.getText());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(resultTokens.begin(), resultTokens.end(),
            expected.begin(), expected.end());
}

BOOST_AUTO_TEST_SUITE_END()
