#include "tests.hpp"

#include "config.hpp"
#include "bilexicon.hpp"

#include <list>

#include <boost/program_options/parsers.hpp>
#include <boost/assign.hpp>

#include <stdio.h>

void testOnLattice(
    BiLexicon& biLexicon,
    const std::string& word,
    const std::string& lexeme,
    const std::string& cat,
    const std::list<std::string>& expectedEntries);

BOOST_AUTO_TEST_SUITE( bilexicon )

BOOST_AUTO_TEST_CASE( bilexicon_simple ) {
    const char* argv[8] = {
        "fakename",
        "--lang",
        "pl",
        "--trg-lang",
        "en",
        "--plain-text-lexicon",
        ROOT_DIR "tools/lexica/bilexicon/t/plen.txt",
        0};

    const int argc = 7;

    boost::program_options::variables_map options;
    // const_casting because of Boost 1.42 bug
    boost::program_options::store(
        boost::program_options::parse_command_line(
            argc, const_cast<char**>(argv), BiLexicon::optionsHandled()), options);
    boost::program_options::notify(options);

    BiLexicon biLexicon(options);
    testOnLattice(biLexicon, "kanału", "kanał+subst", "subst",
                  boost::assign::list_of("channel+subst")("canal+subst"));

    testOnLattice(biLexicon, "garnki", "garnek+subst", "subst",
                  boost::assign::list_of("pot+subst"));

    testOnLattice(biLexicon, "nauskopii", "nauskopia+subst", "subst",
                  std::list<std::string>());

    testOnLattice(biLexicon, "agrafki", "agrafka+subst", "subst",
                  boost::assign::list_of("safety pin+subst"));


}

BOOST_AUTO_TEST_CASE( bilexicon_save_and_load ) {
    char* tmpFile = tempnam(0, "bilexicon_save_and_load_bin");

    {
        const char* argv[10] = {
            "fakename",
            "--lang",
            "pl",
            "--trg-lang",
            "en",
            "--plain-text-lexicon",
            ROOT_DIR "tools/lexica/bilexicon/t/plen.txt",
            "--save-binary-lexicon",
            tmpFile,
            0};

        const int argc = 9;

        boost::program_options::variables_map options;
        boost::program_options::store(
            boost::program_options::parse_command_line(
                argc, const_cast<char**>(argv), BiLexicon::optionsHandled()), options);
        boost::program_options::notify(options);

        BiLexicon biLexicon(options);
        testOnLattice(biLexicon, "kanału", "kanał+subst", "subst",
                      boost::assign::list_of("channel+subst")("canal+subst"));

    }

    {
        const char* argv[8] = {
            "fakename",
            "--lang",
            "pl",
            "--trg-lang",
            "en",
            "--binary-lexicon",
            tmpFile,
            0};

        const int argc = 7;

        boost::program_options::variables_map options;
        boost::program_options::store(
            boost::program_options::parse_command_line(
                argc, const_cast<char**>(argv), BiLexicon::optionsHandled()), options);

        boost::program_options::notify(options);

        BiLexicon biLexicon(options);

        testOnLattice(biLexicon, "kanału", "kanał+subst", "subst",
                      boost::assign::list_of("channel+subst")("canal+subst"));

        testOnLattice(biLexicon, "garnkami", "garnek+subst", "subst",
                      boost::assign::list_of("pot+subst"));

        testOnLattice(biLexicon, "nauskopii", "nauskopia+subst", "subst",
                      std::list<std::string>());

        testOnLattice(biLexicon, "agrafki", "agrafka+subst", "subst",
                      boost::assign::list_of("safety pin+subst"));
    }

    free(tmpFile);
}

BOOST_AUTO_TEST_SUITE_END()

void testOnLattice(
    BiLexicon& biLexicon,
    const std::string& word,
    const std::string& lexeme,
    const std::string& cat,
    const std::list<std::string>& expectedEntries) {

    AnnotationItemManager aim;
    Lattice lattice(aim, word);
    AnnotationItem item(cat, StringFrag(lexeme));

    LayerTagCollection tags = lattice.getLayerTagManager().createTagCollectionFromList(
        boost::assign::list_of("lexeme")("fake"));

    Lattice::EdgeDescriptor edge = lattice.addEdge(
        lattice.getFirstVertex(),
        lattice.getLastVertex(),
        item,
        tags);

    biLexicon.processEdge(lattice, edge);

    std::list<std::string> tagList = boost::assign::list_of("bilexicon");
    Lattice::EdgesSortedBySourceIterator it(lattice, lattice.getLayerTagManager().getMask(tagList));

    BOOST_FOREACH(const std::string& expectedEntry, expectedEntries) {
        BOOST_REQUIRE(it.hasNext());
        Lattice::EdgeDescriptor equivEdge = it.next();

        BOOST_CHECK_EQUAL(lattice.getAnnotationText(equivEdge), expectedEntry);
    }

    BOOST_CHECK(!it.hasNext());
}
