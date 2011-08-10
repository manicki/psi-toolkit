
#include "lattice.hpp"
#include "annotation_item.hpp"

#include "by_spaces_cutter.hpp"

class LatticeTests : public CxxTest::TestSuite
{
public:
    void test_simple() {
        Lattice lattice("Ala ma kota");

        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());

        TS_ASSERT_EQUALS(lattice.getAllText(), "Ala ma kota");

        Lattice::EdgesSortedBySourceIterator ei
            = lattice.edgesSortedBySource(lattice.getLayerTagManager().anyTag());
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'A");
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'l");
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'a");
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "' ");
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'m");
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'a");
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "' ");
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'k");
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'o");
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'t");
        TS_ASSERT(ei.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(ei.next()).getCategory(), "'a");
        TS_ASSERT(!ei.hasNext());

        Lattice::VertexDescriptor pre_ala = lattice.getFirstVertex();
        Lattice::VertexDescriptor post_ala = lattice.getVertexForRawCharIndex(3);
        Lattice::VertexDescriptor pre_ma = lattice.getVertexForRawCharIndex(4);
        Lattice::VertexDescriptor post_ma = lattice.getVertexForRawCharIndex(6);
        Lattice::VertexDescriptor pre_kota = lattice.getVertexForRawCharIndex(7);
        Lattice::VertexDescriptor post_kota = lattice.getLastVertex();

        LayerTagCollection
            raw_tag = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
        LayerTagCollection
            token_tag = lattice.getLayerTagManager().createSingletonTagCollection("token");

        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);

        AnnotationItem word_token("word");
        AnnotationItem blank_token("blank");


        Lattice::Partition ala_partition;
        ala_partition.links.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(0),
            rawMask
        ));
        ala_partition.links.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(1),
            rawMask
        ));
        ala_partition.links.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(2),
            rawMask
        ));
        lattice.addEdge(pre_ala, post_ala, word_token, token_tag, 0, ala_partition);

        Lattice::Partition first_blank_partition;
        first_blank_partition.links.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(3),
            rawMask
        ));
        lattice.addEdge(post_ala, pre_ma, blank_token, token_tag, 0, first_blank_partition);

        Lattice::Partition ma_partition;
        ma_partition.links.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(4),
            rawMask
        ));
        ma_partition.links.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(5),
            rawMask
        ));
        lattice.addEdge(pre_ma, post_ma, word_token, token_tag, 0, ma_partition);

        Lattice::Partition second_blank_partition;
        second_blank_partition.links.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(6),
            rawMask
        ));
        lattice.addEdge(post_ma, pre_kota, blank_token, token_tag, 0, second_blank_partition);

        Lattice::Partition kota_partition;
        kota_partition.links.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(7),
            rawMask
        ));
        kota_partition.links.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(8),
            rawMask
        ));
        kota_partition.links.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(9),
            rawMask
        ));
        kota_partition.links.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(10),
            rawMask
        ));
        lattice.addEdge(pre_kota, post_kota, word_token, token_tag, 0, kota_partition);

        // tests

        Lattice::EdgesSortedBySourceIterator tokenIter = lattice.edgesSortedBySource(tokenMask);
        TS_ASSERT(tokenIter.hasNext());

        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(tokenIter.next()).getCategory(),
            word_token.getCategory()
        );
        TS_ASSERT(tokenIter.hasNext());

        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(tokenIter.next()).getCategory(),
            blank_token.getCategory()
        );
        TS_ASSERT(tokenIter.hasNext());

        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(tokenIter.next()).getCategory(),
            word_token.getCategory()
        );
        TS_ASSERT(tokenIter.hasNext());

        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(tokenIter.next()).getCategory(),
            blank_token.getCategory()
        );
        TS_ASSERT(tokenIter.hasNext());

        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(tokenIter.next()).getCategory(),
            word_token.getCategory()
        );
        TS_ASSERT(!tokenIter.hasNext());

    }

    void test_get_path() {
        Lattice lattice;
        lattice.appendStringWithSymbols("ćma zielona");
        lattice.appendString("<br>");
        lattice.appendStringWithSymbols("mucha");

        LayerTagMask symbolMask = lattice.getLayerTagManager().getMask("symbol");

        Lattice::VertexDescriptor vertex = lattice.getFirstVertex();

        Lattice::Partition partition = lattice.getPath(vertex, symbolMask);

        Lattice::InOutEdgesIterator iter = lattice.outEdges(vertex, symbolMask);
        TS_ASSERT(!iter.hasNext());

        TS_ASSERT_EQUALS(partition.size(), 11);
    }

    void bla_test_cutter() {
        Lattice lattice;
        lattice.appendStringWithSymbols("zielony rower");

        BySpacesCutter cutter;

        LayerTagMask symbolMask = lattice.getLayerTagManager().getMask("symbol");

        lattice.runCutter(cutter, symbolMask);

        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask("token");

        Lattice::EdgesSortedBySourceIterator tokenIter = lattice.edgesSortedBySource(tokenMask);

        TS_ASSERT(tokenIter.hasNext());
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(tokenIter.next()).getCategory(),
            "rower");

        TS_ASSERT(tokenIter.hasNext());
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(tokenIter.next()).getCategory(),
            " ");

        TS_ASSERT(tokenIter.hasNext());
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(tokenIter.next()).getCategory(),
            "zielony");

        TS_ASSERT(!tokenIter.hasNext());
    }

};
