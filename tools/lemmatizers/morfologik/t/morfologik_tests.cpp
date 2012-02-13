#include "tests.hpp"

#include "morfologik.hpp"
#include "by_spaces_cutter.hpp"

#include "lemmatizer_annotator.hpp"

BOOST_AUTO_TEST_SUITE( morfologik )

BOOST_AUTO_TEST_CASE( simple_stem ) {
    boost::program_options::variables_map noOptions;
    Morfologik morf(noOptions);

    std::vector<std::string> stems = morf.simpleStem("dziecku");
    BOOST_CHECK_EQUAL((int)stems.size(), 1);

    std::vector<std::string>::iterator s = stems.begin();
    BOOST_CHECK_EQUAL(*s, "dziecko");
}

BOOST_AUTO_TEST_CASE( unrecognized ) {
    boost::program_options::variables_map noOptions;
    Morfologik morf(noOptions);

    std::vector<std::string> stems = morf.simpleStem("Dziecko");
    BOOST_CHECK_EQUAL((int)stems.size(), 0);
}

BOOST_AUTO_TEST_CASE( stems ) {
    boost::program_options::variables_map noOptions;
    Morfologik morf(noOptions);

    std::multimap<std::string, std::vector<std::string> > stems =
        morf.stem("jeden");
    BOOST_CHECK_EQUAL((int)stems.size(), 2);

    std::multimap<std::string, std::vector<std::string> >::iterator i =
        stems.begin();

    std::string s1 = i->first;
    std::vector<std::string> v1 = i->second;
    ++i;
    std::string s2 = i->first;
    std::vector<std::string> v2 = i->second;

    BOOST_CHECK_EQUAL(s1, s2);
    BOOST_CHECK((int)v1.size() != (int)v2.size());
    BOOST_CHECK(v1 != v2);
}

BOOST_AUTO_TEST_CASE( lexeme_level ) {

    Lattice lattice;
    std::string paragraph = "prowokacjami";
    lattice.appendStringWithSymbols(paragraph);

    BySpacesCutter cutter;
    LayerTagMask symbolMask = lattice.getLayerTagManager().getMask("symbol");
    LayerTagMask textMask = lattice.getLayerTagManager().getMask("text");

    LayerTagCollection textTags(
        lattice.getLayerTagManager().createSingletonTagCollection("text"));
    AnnotationItem item("TEXT", paragraph);
    lattice.addEdge(lattice.getFirstVertex(), lattice.getLastVertex(),
                    item, textTags);

    lattice.runCutter(cutter, symbolMask, textMask);

    boost::program_options::variables_map noOptions;
    LemmatizerAnnotator<Morfologik> annotator(noOptions);
    annotator.annotate(lattice);

    // now checking
    {
        LayerTagMask lemmaMask_ = lattice.getLayerTagManager().getMask("lexeme");
        Lattice::EdgesSortedByTargetIterator lemmaIter = lattice.edgesSortedByTarget(lemmaMask_);

        BOOST_CHECK(lemmaIter.hasNext());

        Lattice::EdgeDescriptor prowokacjamiLemma = lemmaIter.next();
        AnnotationItem prowokacjamiItem = lattice.getEdgeAnnotationItem(prowokacjamiLemma);

        BOOST_CHECK_EQUAL(prowokacjamiItem.getCategory(), "subst");
        BOOST_CHECK_EQUAL(prowokacjamiItem.getText(), "prowokacja+subst");

        AnnotationItemManager annItmMng = lattice.getAnnotationItemManager();
        std::list<std::pair<std::string, std::string> > lexemeValues =
            annItmMng.getValues(prowokacjamiItem);

        BOOST_CHECK_EQUAL((int)lexemeValues.size(), 1);
        std::list<std::pair<std::string, std::string> >::iterator valItr = lexemeValues.begin();

        BOOST_CHECK_EQUAL(valItr->first, "gender");
        BOOST_CHECK_EQUAL(valItr->second, "f");

        BOOST_CHECK(!lemmaIter.hasNext());
    }

    {
        LayerTagMask formMask_ = lattice.getLayerTagManager().getMask("form");
        Lattice::EdgesSortedByTargetIterator formIter = lattice.edgesSortedByTarget(formMask_);

        BOOST_CHECK(formIter.hasNext());

        Lattice::EdgeDescriptor prowokacjamiForm = formIter.next();
        AnnotationItem prowokacjamiItem = lattice.getEdgeAnnotationItem(prowokacjamiForm);

        BOOST_CHECK_EQUAL(prowokacjamiItem.getCategory(), "subst");
        BOOST_CHECK_EQUAL(prowokacjamiItem.getText(), "prowokacjami");

        AnnotationItemManager annItmMng = lattice.getAnnotationItemManager();
        std::list<std::pair<std::string, std::string> > formValues =
            annItmMng.getValues(prowokacjamiItem);

        BOOST_CHECK_EQUAL((int)formValues.size(), 3);
        std::list<std::pair<std::string, std::string> >::iterator valItr = formValues.begin();

        BOOST_CHECK_EQUAL(valItr->first, "gender");
        BOOST_CHECK_EQUAL(valItr->second, "f");
        ++valItr;
        BOOST_CHECK_EQUAL(valItr->first, "case");
        BOOST_CHECK_EQUAL(valItr->second, "inst");
        ++valItr;
        BOOST_CHECK_EQUAL(valItr->first, "number");
        BOOST_CHECK_EQUAL(valItr->second, "pl");

        BOOST_CHECK(!formIter.hasNext());
    }
}

BOOST_AUTO_TEST_CASE( many_lexemes ) {

    Lattice lattice;
    std::string paragraph = "mam";
    lattice.appendStringWithSymbols(paragraph);

    BySpacesCutter cutter;
    LayerTagMask symbolMask = lattice.getLayerTagManager().getMask("symbol");
    LayerTagMask textMask = lattice.getLayerTagManager().getMask("text");

    LayerTagCollection textTags(
        lattice.getLayerTagManager().createSingletonTagCollection("text"));
    AnnotationItem item("TEXT", paragraph);
    lattice.addEdge(lattice.getFirstVertex(), lattice.getLastVertex(),
                    item, textTags);

    lattice.runCutter(cutter, symbolMask, textMask);

    boost::program_options::variables_map noOptions;
    LemmatizerAnnotator<Morfologik> annotator(noOptions);
    annotator.annotate(lattice);

    // now checking
    {
        LayerTagMask lemmaMask_ = lattice.getLayerTagManager().getMask("lexeme");
        Lattice::EdgesSortedByTargetIterator lemmaIter = lattice.edgesSortedByTarget(lemmaMask_);

        BOOST_CHECK(lemmaIter.hasNext());

        Lattice::EdgeDescriptor lemma = lemmaIter.next();
        AnnotationItem item = lattice.getEdgeAnnotationItem(lemma);

        BOOST_CHECK_EQUAL(item.getCategory(), "subst");
        BOOST_CHECK_EQUAL(item.getText(), "mama+subst");

        BOOST_CHECK(lemmaIter.hasNext());

        lemma = lemmaIter.next();
        item = lattice.getEdgeAnnotationItem(lemma);

        BOOST_CHECK_EQUAL(item.getCategory(), "verb");
        BOOST_CHECK_EQUAL(item.getText(), "mamić+verb");
        BOOST_CHECK(lemmaIter.hasNext());

        lemma = lemmaIter.next();
        item = lattice.getEdgeAnnotationItem(lemma);

        BOOST_CHECK_EQUAL(item.getCategory(), "verb");
        BOOST_CHECK_EQUAL(item.getText(), "mieć+verb");

        BOOST_CHECK(!lemmaIter.hasNext());
    }
}

BOOST_AUTO_TEST_SUITE_END()
