#include <iostream>

#include "psi_lattice_writer.hpp"
#include "utt_lattice_reader.hpp"


class LatticeReadersWritersTests : public CxxTest::TestSuite {

public:

    void testUTTLatticeReader() {

        Lattice lattice("");
        LatticeReader * reader = new UTTLatticeReader();
/*
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
*/
    }

    void testPsiLatticeWriterSimple() {

        Lattice lattice("Ala ma kota");
        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());

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
        lattice.getAnnotationItemManager().setValue(word_token, "type", "word");

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

        LatticeWriter * writer = new PsiLatticeWriter();

        std::ostringstream osstr;
        writer->writeLattice(lattice, osstr);

        std::string line;
        std::string contents;
        std::ifstream s("../formats/psi/t/files/pl_sample_simple.txt");
        while (getline(s, line)) {
            contents += line;
            contents += "\n";
        }

        TS_ASSERT_EQUALS(osstr.str(), contents);

    }

    void testPsiLatticeWriterAdvanced() {

        Lattice lattice("Ala ma&nbsp;<b>kta</b>");
        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());

        Lattice::VertexDescriptor preAla = lattice.getFirstVertex();
        Lattice::VertexDescriptor postAla = lattice.getVertexForRawCharIndex(3);

        LayerTagCollection
            rawTag = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
        LayerTagCollection
            tokenTag = lattice.getLayerTagManager().createSingletonTagCollection("token");

        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(rawTag);
        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(tokenTag);

        AnnotationItem aiAla("'Ala'");
        lattice.getAnnotationItemManager().setValue(aiAla, "type", "word");

        Lattice::Partition partitionAla;
        partitionAla.links.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(0),
            rawMask
        ));
        partitionAla.links.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(1),
            rawMask
        ));
        partitionAla.links.push_back(lattice.firstOutEdge(
            lattice.getVertexForRawCharIndex(2),
            rawMask
        ));

        lattice.addEdge(preAla, postAla, aiAla, tokenTag, 0, partitionAla);

        LatticeWriter * writer = new PsiLatticeWriter();

        writer->writeLattice(lattice, std::cout);
/*
        std::ostringstream osstr;
        writer->writeLattice(lattice, osstr);

        std::string line;
        std::string contents;
        std::ifstream s("../formats/psi/t/files/pl_sample_nocomments.txt");
        while (getline(s, line)) {
            contents += line;
            contents += "\n";
        }

        TS_ASSERT_EQUALS(osstr.str(), contents);
*/
    }

};
