#include "tests.hpp"

#include <boost/scoped_ptr.hpp>

#include "utt_lattice_reader.hpp"


BOOST_AUTO_TEST_SUITE( utt_lattice_reader )

BOOST_AUTO_TEST_CASE( utt_lattice_reader ) {
    AnnotationItemManager aim;
    Lattice lattice(aim, "");

    boost::scoped_ptr<StreamLatticeReader> reader(new UTTLatticeReader());

    BOOST_CHECK_EQUAL(reader->getFormatName(), "UTT");
    BOOST_CHECK_EQUAL(reader->info(), "UTT reader");

    reader->readIntoLattice("../formats/utt/t/files/fr_simple_puddle_input.txt", lattice);

    LayerTagMask rawMask = lattice.getLayerTagManager().getMask("symbol");
    Lattice::EdgesSortedBySourceIterator rei = lattice.edgesSortedBySource(rawMask);

    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'A");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'m");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'e");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'l");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'i");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'e");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "' ");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'a");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "' ");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'u");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'n");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "' ");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'c");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'h");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'a");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'t");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'.");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'Q");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'u");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'e");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'l");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'l");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'e");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "' ");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'â");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'g");
    BOOST_CHECK(rei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(rei.next()), "'e");

    LayerTagMask tokenMask = lattice.getLayerTagManager().getMask("token");
    Lattice::EdgesSortedBySourceIterator tei = lattice.edgesSortedBySource(tokenMask);

    BOOST_CHECK(tei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(tei.next()), "'Amelie'");
    BOOST_CHECK(tei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(tei.next()), "' '");
    BOOST_CHECK(tei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(tei.next()), "'a'");
    BOOST_CHECK(tei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(tei.next()), "' '");
    BOOST_CHECK(tei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(tei.next()), "'un'");
    BOOST_CHECK(tei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(tei.next()), "' '");
    BOOST_CHECK(tei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(tei.next()), "'chat'");
    BOOST_CHECK(tei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(tei.next()), "'.'");
    BOOST_CHECK(tei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(tei.next()), "'Quelle'");
    BOOST_CHECK(tei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(tei.next()), "' '");
    BOOST_CHECK(tei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationCategory(tei.next()), "'âge'");

    LayerTagMask sentenceMask = lattice.getLayerTagManager().getMask("sentence");
    Lattice::EdgesSortedBySourceIterator sei = lattice.edgesSortedBySource(sentenceMask);

    BOOST_CHECK(sei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationText(sei.next()),
        "Amelie a un chat.");
    BOOST_CHECK(sei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationText(sei.next()),
        "Quelle âge as-tu?");
    BOOST_CHECK(sei.hasNext());
    BOOST_CHECK_EQUAL(lattice.getAnnotationText(sei.next()),
        "Karol Józef Wojtyła est élu pape de l'Église catholique romaine le 16 octobre 1978.");
    BOOST_CHECK(!sei.hasNext());
}


BOOST_AUTO_TEST_CASE( utt_lattice_reader_corrupted ) {

    AnnotationItemManager aim;
    Lattice lattice(aim, "");

    boost::scoped_ptr<StreamLatticeReader> reader(new UTTLatticeReader());

    BOOST_CHECK_THROW(
        reader->readIntoLattice(ROOT_DIR "formats/utt/t/files/corrupted.txt", lattice),
        FileFormatException
    );

}


BOOST_AUTO_TEST_SUITE_END()
