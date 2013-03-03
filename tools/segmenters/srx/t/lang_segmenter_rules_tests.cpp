#include "tests.hpp"

#include "lattice.hpp"
#include "annotation_item_manager.hpp"
#include "srx_segmenter.hpp"
#include <boost/assign.hpp>

BOOST_AUTO_TEST_SUITE( srx_segmenter_rules )

void initTestLattice_(Lattice &lattice, const std::string &text,
        const std::string &lang);
void test_segmenter(const std::string &lang, const std::string &text,
        std::list<std::string> expected);

BOOST_AUTO_TEST_CASE( finnish_segmenter_rules ) {
    std::list<std::string> expectedSegments = boost::assign::list_of
        ("Tämä juna jatkaa Vantaankoskelle.")
        (" Pidän mm. appelsiinista, omenasta ja makkarasta.")
        (" Täällä on kirahvia, norsuja, virtahepoja jne.");
    test_segmenter("fi",
            "Tämä juna jatkaa Vantaankoskelle. Pidän mm. appelsiinista, omenasta ja makkarasta. Täällä on kirahvia, norsuja, virtahepoja jne.",
            expectedSegments);
}

BOOST_AUTO_TEST_CASE( swedish_segmenter_rules ) {
    std::list<std::string> expectedSegments = boost::assign::list_of
        ("Det här tåget förtsätter till Vandaforsen.")
        (" I Polen bor ca. 38 miljoner människor.")
        (" I Stockholm kan man t.ex. äta köttbullar.");
    test_segmenter("sv",
            "Det här tåget förtsätter till Vandaforsen. I Polen bor ca. 38 miljoner människor. I Stockholm kan man t.ex. äta köttbullar.",
            expectedSegments);
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

void test_segmenter(const std::string &lang, const std::string &text,
        std::list<std::string> expected) {
    AnnotationItemManager aim;
    Lattice lattice(aim);
    initTestLattice_(lattice, text, lang);

    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);
    boost::filesystem::path rules = fileFetcher.getOneFile(
            ROOT_DIR "tools/segmenters/srx/data/" + lang + "/segmentation.srx");

    SrxSegmenter segmenter(
                lang,
                rules,
                1000,
                600,
                false);
    segmenter.annotate(lattice);

    std::list<std::string> resultSegments;
    LayerTagMask segmentMask = lattice.getLayerTagManager().getMask("segment");
    Lattice::EdgesSortedByTargetIterator segmentIter = lattice.edgesSortedByTarget(segmentMask);
    while (segmentIter.hasNext()) {
        Lattice::EdgeDescriptor edge = segmentIter.next();
        AnnotationItem annotationItem = lattice.getEdgeAnnotationItem(edge);
        resultSegments.push_back(annotationItem.getText());
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(resultSegments.begin(), resultSegments.end(),
            expected.begin(), expected.end());
}

BOOST_AUTO_TEST_SUITE_END()
