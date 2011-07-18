
#include "lattice.hpp"
#include "annotation_item.hpp"

class LatticeTests : public CxxTest::TestSuite
{
public:
    void test_simple() {
        Lattice lattice("Ala ma kota");

        std::list<Lattice::EdgeDescriptor>
            allEdges = lattice.edgesSorted(lattice.getLayerTagManager().anyTag());
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

        LayerTagCollection
            raw_tag = lattice.getLayerTagManager().createSingletonTagCollection("raw");
        LayerTagCollection
            token_tag = lattice.getLayerTagManager().createSingletonTagCollection("token");

        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);

        AnnotationItem word_token("word");
        AnnotationItem blank_token("blank");


        std::list<Lattice::EdgeDescriptor> ala_partition;
        ala_partition.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(0),
            rawMask
        ));
        ala_partition.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(1),
            rawMask
        ));
        ala_partition.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(2),
            rawMask
        ));
        lattice.addEdge(pre_ala, post_ala, word_token, token_tag, 0, ala_partition);

        std::list<Lattice::EdgeDescriptor> first_blank_partition;
        first_blank_partition.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(3),
            rawMask
        ));
        lattice.addEdge(post_ala, pre_ma, blank_token, token_tag, 0, first_blank_partition);

        std::list<Lattice::EdgeDescriptor> ma_partition;
        ma_partition.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(4),
            rawMask
        ));
        ma_partition.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(5),
            rawMask
        ));
        lattice.addEdge(pre_ma, post_ma, word_token, token_tag, 0, ma_partition);

        std::list<Lattice::EdgeDescriptor> second_blank_partition;
        second_blank_partition.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(6),
            rawMask
        ));
        lattice.addEdge(post_ma, pre_kota, blank_token, token_tag, 0, second_blank_partition);

        std::list<Lattice::EdgeDescriptor> kota_partition;
        kota_partition.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(7),
            rawMask
        ));
        kota_partition.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(8),
            rawMask
        ));
        kota_partition.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(9),
            rawMask
        ));
        kota_partition.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(10),
            rawMask
        ));
        lattice.addEdge(pre_kota, post_kota, word_token, token_tag, 0, kota_partition);

        // tests

        std::list<Lattice::EdgeDescriptor> tokens = lattice.edgesSorted(tokenMask);
        std::list<Lattice::EdgeDescriptor>::iterator token_iter = tokens.begin();
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(*token_iter).getCategory(),
            word_token.getCategory()
        );

        ++token_iter;
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(*token_iter).getCategory(),
            blank_token.getCategory()
        );

        ++token_iter;
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(*token_iter).getCategory(),
            word_token.getCategory()
        );

        ++token_iter;
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(*token_iter).getCategory(),
            blank_token.getCategory()
        );

        ++token_iter;
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(*token_iter).getCategory(),
            word_token.getCategory()
        );

        ++token_iter;
        TS_ASSERT_EQUALS(token_iter, tokens.end());

    }

    void test_puddle() {
        Lattice lattice("blanc chat");
        LayerTagCollection raw_tag = lattice.getLayerTagManager().createSingletonTagCollection("raw");
        LayerTagCollection token_tag = lattice.getLayerTagManager().createSingletonTagCollection("token");
        LayerTagCollection lemma_tag = lattice.getLayerTagManager().createSingletonTagCollection("lemma");
        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
        LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
        AnnotationItem word_token("word");
        AnnotationItem blank_token("blank");
        AnnotationItem lemma_token("lemma"); // temporary, as long as annotation items are only distinguished by their 'name'

        Lattice::VertexDescriptor pre_blanc = lattice.getFirstVertex();
        Lattice::VertexDescriptor post_blanc = lattice.getVertexForRawCharIndex(5);
        Lattice::VertexDescriptor pre_chat = lattice.getVertexForRawCharIndex(6);
        Lattice::VertexDescriptor post_chat = lattice.getLastVertex();

        std::list<Lattice::EdgeDescriptor>
            allEdges = lattice.edgesSorted(lattice.getLayerTagManager().anyTag());
        std::list<Lattice::EdgeDescriptor>::iterator ei = allEdges.begin();
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), "b");
        ++ei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), "l");
        ++ei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), "a");
        ++ei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), "n");
        ++ei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), "c");
        ++ei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), " ");
        ++ei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), "c");
        ++ei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), "h");
        ++ei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), "a");
        ++ei;
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(*ei).getCategory(), "t");

        std::list<Lattice::EdgeDescriptor> blanc_partition;
        for (int i = 0; i < 5; i ++) {
            blanc_partition.push_back(lattice.firstOutEdge(
                        lattice.getVertexForRawCharIndex(i),
                        rawMask
                        ));
        }
        lattice.addEdge(pre_blanc, post_blanc, word_token, token_tag, 0, blanc_partition);
        std::list<Lattice::EdgeDescriptor> blank_partition;
        blank_partition.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(6),
            rawMask
        ));
        lattice.addEdge(post_blanc, pre_chat, blank_token, token_tag, 0, blank_partition);
        std::list<Lattice::EdgeDescriptor> chat_partition;
        for (int i = 6; i < 10; i ++) {
            chat_partition.push_back(lattice.firstOutEdge(
                        lattice.getVertexForRawCharIndex(i),
                        rawMask
                        ));
        }
        lattice.addEdge(pre_chat, post_chat, word_token, token_tag, 0, chat_partition);

        std::list<Lattice::EdgeDescriptor> blanc_lemma_partition;
        blanc_lemma_partition.push_back(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
        lattice.addEdge(pre_blanc, post_blanc, lemma_token, lemma_tag, 0, blanc_lemma_partition);

        std::list<Lattice::EdgeDescriptor> chat_lemma_partition;
        chat_lemma_partition.push_back(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(6), tokenMask));
        lattice.addEdge(pre_chat, post_chat, lemma_token, lemma_tag, 0, chat_lemma_partition);

        std::list<Lattice::EdgeDescriptor> tokens = lattice.edgesSorted(lemmaMask);
        std::list<Lattice::EdgeDescriptor>::iterator token_it = tokens.begin();
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(*token_it).getCategory(),
            lemma_token.getCategory()
        );
        token_it ++;
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(*token_it).getCategory(),
            lemma_token.getCategory()
        );
        token_it ++;
        TS_ASSERT_EQUALS(token_it, tokens.end());
    }

};
