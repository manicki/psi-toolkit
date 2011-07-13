
#include "lattice.hpp"
#include "annotation_item.hpp"

class LatticeTests : public CxxTest::TestSuite
{
public:
    void test_simple() {
        Lattice lattice("Ala ma kota");

        std::list<Lattice::EdgeDescriptor> allEdges = lattice.edgesSorted(lattice.getLayerTagManager().getAllTags());
        std::list<Lattice::EdgeDescriptor>::iterator ei = allEdges.begin();
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

        Lattice::VertexDescriptor pre_ala = lattice.getFirstVertex();
        Lattice::VertexDescriptor post_ala = lattice.getVertexForRawCharIndex(3);
        Lattice::VertexDescriptor pre_ma = lattice.getVertexForRawCharIndex(4);
        Lattice::VertexDescriptor post_ma = lattice.getVertexForRawCharIndex(6);
        Lattice::VertexDescriptor pre_kota = lattice.getVertexForRawCharIndex(7);
        Lattice::VertexDescriptor post_kota = lattice.getLastVertex();

        LayerTagCollection raw_tag = lattice.getLayerTagManager().createSingletonTagCollection("raw");
        LayerTagCollection token_tag = lattice.getLayerTagManager().createSingletonTagCollection("token");

        AnnotationItem word_token("word");
        AnnotationItem blank_token("blank");


        std::list<Lattice::EdgeDescriptor> ala_partition;
        ala_partition.push_back(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), raw_tag));
        ala_partition.push_back(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(1), raw_tag));
        ala_partition.push_back(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(2), raw_tag));
        lattice.addEdge(pre_ala, post_ala, word_token, token_tag, ala_partition);

        std::list<Lattice::EdgeDescriptor> first_blank_partition;
        first_blank_partition.push_back(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(3), raw_tag));
        lattice.addEdge(post_ala, pre_ma, blank_token, token_tag, first_blank_partition);

        std::list<Lattice::EdgeDescriptor> ma_partition;
        ma_partition.push_back(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(4), raw_tag));
        ma_partition.push_back(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(5), raw_tag));
        lattice.addEdge(pre_ma, post_ma, word_token, token_tag, ma_partition);

        std::list<Lattice::EdgeDescriptor> second_blank_partition;
        second_blank_partition.push_back(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(6), raw_tag));
        lattice.addEdge(post_ma, pre_kota, blank_token, token_tag, second_blank_partition);

        std::list<Lattice::EdgeDescriptor> kota_partition;
        kota_partition.push_back(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(7), raw_tag));
        kota_partition.push_back(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(8), raw_tag));
        kota_partition.push_back(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(9), raw_tag));
        kota_partition.push_back(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(10), raw_tag));
        lattice.addEdge(pre_kota, post_kota, word_token, token_tag, kota_partition);

        // tests

        std::list<Lattice::EdgeDescriptor> tokens = lattice.edgesSorted(token_tag);
        std::list<Lattice::EdgeDescriptor>::iterator token_iter = tokens.begin();
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*token_iter).getCategory(), word_token.getCategory());

        ++token_iter;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*token_iter).getCategory(), blank_token.getCategory());

        ++token_iter;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*token_iter).getCategory(), word_token.getCategory());

        ++token_iter;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*token_iter).getCategory(), blank_token.getCategory());

        ++token_iter;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*token_iter).getCategory(), word_token.getCategory());

        ++token_iter;
        TS_ASSERT_EQUALS(token_iter, tokens.end());

    }

};
