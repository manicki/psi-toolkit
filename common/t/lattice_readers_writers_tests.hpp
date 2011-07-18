#include "psi_lattice_reader.hpp"
#include "utt_lattice_reader.hpp"


class LatticeReadersWritersTests : public CxxTest::TestSuite {

public:

    void testUTTLatticeReader() {

        Lattice lattice("");
        LatticeReader * reader = new UTTLatticeReader();

        reader->readIntoLattice("../test/files/utt/fr_simple_puddle_input.txt", lattice);

        LayerTagMask rawMask
            = lattice.getLayerTagManager().getMask("raw");
        std::list<Lattice::EdgeDescriptor> rawEdges = lattice.edgesSorted(rawMask);
        std::list<Lattice::EdgeDescriptor>::iterator rei = rawEdges.begin();

        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "A");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "m");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "e");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "l");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "i");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "e");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), " ");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "a");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), " ");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "u");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "n");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), " ");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "c");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "h");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "a");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "t");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), ".");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "Q");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "u");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "e");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "l");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "l");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "e");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), " ");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "â");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "g");
        ++rei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*rei).getCategory(), "e");

        LayerTagMask tokenMask
            = lattice.getLayerTagManager().getMask("token");
        std::list<Lattice::EdgeDescriptor> tokenEdges = lattice.edgesSorted(tokenMask);
        std::list<Lattice::EdgeDescriptor>::iterator tei = tokenEdges.begin();

        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*tei).getCategory(), "Amelie");
        ++tei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*tei).getCategory(), " ");
        ++tei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*tei).getCategory(), "a");
        ++tei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*tei).getCategory(), " ");
        ++tei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*tei).getCategory(), "un");
        ++tei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*tei).getCategory(), " ");
        ++tei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*tei).getCategory(), "chat");
        ++tei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*tei).getCategory(), ".");
        ++tei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*tei).getCategory(), "Quelle");
        ++tei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*tei).getCategory(), " ");
        ++tei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*tei).getCategory(), "âge");

        LayerTagMask sentenceMask
            = lattice.getLayerTagManager().getMask("sentence");
        std::list<Lattice::EdgeDescriptor> sentenceEdges = lattice.edgesSorted(sentenceMask);
        std::list<Lattice::EdgeDescriptor>::iterator sei = sentenceEdges.begin();

        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*sei).getCategory(), "Amelie a un chat.");
        ++sei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*sei).getCategory(), "Quelle âge as-tu?");
        ++sei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*sei).getCategory(),
            "Karol Józef Wojtyła est élu pape de l'Église catholique romaine le 16 octobre 1978.");

        delete reader;

    }

    void testPsiLatticeReader() {

        Lattice lattice("");
        LatticeReader * reader = new PsiLatticeReader();

        reader->readIntoLattice("../test/files/psi/pl_sample.txt.txt", lattice);

        delete reader;

    }

};
