#include "psi_lattice_reader.hpp"
#include "utt_lattice_reader.hpp"


class LatticeReadersWritersTests : public CxxTest::TestSuite {

public:

    void testUTTLatticeReader() {

        Lattice lattice("");
        LatticeReader * reader = new UTTLatticeReader();

        reader->readIntoLattice("../formats/utt/t/files/fr_simple_puddle_input.txt", lattice);

        LayerTagMask rawMask = lattice.getLayerTagManager().getMask("raw");
        Lattice::SortedEdgesIterator rei = lattice.edgesSorted(rawMask);

        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "A");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "m");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "e");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "l");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "i");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "e");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), " ");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "a");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), " ");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "u");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "n");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), " ");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "c");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "h");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "a");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "t");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), ".");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "Q");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "u");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "e");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "l");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "l");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "e");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), " ");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "â");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "g");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(rei.next()).getCategory(), "e");

        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask("token");
        Lattice::SortedEdgesIterator tei = lattice.edgesSorted(tokenMask);

        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(tei.next()).getCategory(), "Amelie");
        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(tei.next()).getCategory(), " ");
        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(tei.next()).getCategory(), "a");
        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(tei.next()).getCategory(), " ");
        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(tei.next()).getCategory(), "un");
        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(tei.next()).getCategory(), " ");
        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(tei.next()).getCategory(), "chat");
        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(tei.next()).getCategory(), ".");
        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(tei.next()).getCategory(), "Quelle");
        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(tei.next()).getCategory(), " ");
        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(tei.next()).getCategory(), "âge");

        LayerTagMask sentenceMask = lattice.getLayerTagManager().getMask("sentence");
        Lattice::SortedEdgesIterator sei = lattice.edgesSorted(sentenceMask);

        TS_ASSERT(sei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(sei.next()).getCategory(),
            "Amelie a un chat.");
        TS_ASSERT(sei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(sei.next()).getCategory(),
            "Quelle âge as-tu?");
        TS_ASSERT(sei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(sei.next()).getCategory(),
            "Karol Józef Wojtyła est élu pape de l'Église catholique romaine le 16 octobre 1978.");
        TS_ASSERT(!sei.hasNext());

        delete reader;

    }

    void testPsiLatticeReader() {

        Lattice lattice("");
        LatticeReader * reader = new PsiLatticeReader();

        reader->readIntoLattice("../formats/psi/t/files/pl_sample.txt", lattice);

        delete reader;

    }

};
