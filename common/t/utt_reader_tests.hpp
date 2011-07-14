#include "utt_reader.hpp"


class UTTReaderTests : public CxxTest::TestSuite {

public:

    void test_simple() {
        
        Lattice lattice("");
        LatticeReader * reader = new UTTReader();
        
        reader->readIntoLattice("../../formats/utt/t/files/fr_simple_puddle_input.txt", lattice);
        
        LayerTagCollection tokenTag 
            = lattice.getLayerTagManager().createSingletonTagCollection("token");
        std::list<Lattice::EdgeDescriptor> edges = lattice.edgesSorted(tokenTag);
        std::list<Lattice::EdgeDescriptor>::iterator ei = edges.begin();
        
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), "A");
        ++ei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), "l");
        ++ei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), "a");
        ++ei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), " ");
        ++ei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), "m");
        ++ei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), "a");
        ++ei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), " ");
        ++ei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), "k");
        ++ei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), "o");
        ++ei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), "t");
        ++ei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), "a");
        
        delete reader;
        
    }

};
