#include <iostream>

#include <boost/scoped_ptr.hpp>

#include "psi_lattice_reader.hpp"
#include "psi_lattice_writer.hpp"
#include "simple_lattice_writer.hpp"
#include "utt_lattice_reader.hpp"

#include "config.h"

class LatticeReadersWritersTests : public CxxTest::TestSuite {

public:

    void testUTTLatticeReader() {

        Lattice lattice("");
/*
        LatticeReader * reader = new UTTLatticeReader();

        reader->readIntoLattice("../formats/utt/t/files/fr_simple_puddle_input.txt", lattice);

        LayerTagMask rawMask = lattice.getLayerTagManager().getMask("raw");
        Lattice::EdgesSortedBySourceIterator rei = lattice.edgesSortedBySource(rawMask);

        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "A");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "m");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "e");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "l");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "i");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "e");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), " ");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "a");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), " ");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "u");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "n");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), " ");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "c");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "h");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "a");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "t");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), ".");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "Q");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "u");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "e");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "l");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "l");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "e");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), " ");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "â");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "g");
        TS_ASSERT(rei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(rei.next()), "e");

        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask("token");
        Lattice::EdgesSortedBySourceIterator tei = lattice.edgesSortedBySource(tokenMask);

        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(tei.next()), "Amelie");
        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(tei.next()), " ");
        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(tei.next()), "a");
        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(tei.next()), " ");
        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(tei.next()), "un");
        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(tei.next()), " ");
        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(tei.next()), "chat");
        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(tei.next()), ".");
        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(tei.next()), "Quelle");
        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(tei.next()), " ");
        TS_ASSERT(tei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(tei.next()), "âge");

        LayerTagMask sentenceMask = lattice.getLayerTagManager().getMask("sentence");
        Lattice::EdgesSortedBySourceIterator sei = lattice.edgesSortedBySource(sentenceMask);

        TS_ASSERT(sei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(sei.next()),
            "Amelie a un chat.");
        TS_ASSERT(sei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(sei.next()),
            "Quelle âge as-tu?");
        TS_ASSERT(sei.hasNext());
        TS_ASSERT_EQUALS(lattice.getAnnotationCategory(sei.next()),
            "Karol Józef Wojtyła est élu pape de l'Église catholique romaine le 16 octobre 1978.");
        TS_ASSERT(!sei.hasNext());

        delete reader;
*/
    }


    void testPsiLatticeWriterSimple() {

        Lattice lattice;
        prepareSimpleLattice_(lattice);

        boost::scoped_ptr<LatticeWriter> writer(new PsiLatticeWriter());

        std::ostringstream osstr;
        writer->writeLattice(lattice, osstr);

        std::string line;
        std::string contents;
        std::ifstream s(ROOT_DIR "formats/psi/t/files/pl_sample_simple.txt");
        while (getline(s, line)) {
            contents += line;
            contents += "\n";
        }

        TS_ASSERT_EQUALS(osstr.str(), contents);

    }


    void testPsiLatticeWriterAdvanced() {

        Lattice lattice;
        prepareAdvancedLattice_(lattice);

        boost::scoped_ptr<LatticeWriter> writer(new PsiLatticeWriter());

        // writer->writeLattice(lattice, std::cout);

        std::ostringstream osstr;
        writer->writeLattice(lattice, osstr);

        std::string line;
        std::string contents;
        std::ifstream s(ROOT_DIR "formats/psi/t/files/pl_sample_nocomments.txt");
        while (getline(s, line)) {
            contents += line;
            contents += "\n";
        }

        TS_ASSERT_EQUALS(osstr.str(), contents);
    }


    void testPsiLatticeReader() {

        Lattice lattice("");

        boost::scoped_ptr<LatticeReader> reader(new PsiLatticeReader());

        reader->readIntoLattice(ROOT_DIR "formats/psi/t/files/pl_sample.txt", lattice);
    }


    void testSimpleLatticeWriter() {

        Lattice lattice;
        prepareSimpleLattice_(lattice);

        boost::scoped_ptr<LatticeWriter> writer(new SimpleLatticeWriter());

        std::ostringstream osstr;
        writer->writeLattice(lattice, osstr);

        std::string line;
        std::string contents;
        std::ifstream s(ROOT_DIR "formats/simple/t/files/simple_ala.txt");
        while (getline(s, line)) {
            contents += line;
            contents += "\n";
        }

        TS_ASSERT_EQUALS(osstr.str(), contents);

    }


private:

    void prepareSimpleLattice_(Lattice & lattice) {

        lattice.appendString("Ala ma słonia");
        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());

        Lattice::VertexDescriptor pre_ala = lattice.getFirstVertex();
        Lattice::VertexDescriptor post_ala = lattice.getVertexForRawCharIndex(3);
        Lattice::VertexDescriptor pre_ma = lattice.getVertexForRawCharIndex(4);
        Lattice::VertexDescriptor post_ma = lattice.getVertexForRawCharIndex(6);
        Lattice::VertexDescriptor pre_slonia = lattice.getVertexForRawCharIndex(7);
        Lattice::VertexDescriptor post_slonia = lattice.getLastVertex();

        LayerTagCollection
            raw_tag = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
        LayerTagCollection
            token_tag = lattice.getLayerTagManager().createSingletonTagCollection("token");

        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);

        AnnotationItem word_token("word");
        lattice.getAnnotationItemManager().setValue(word_token, "type", "word");

        AnnotationItem blank_token("blank");

        {
            Lattice::EdgeSequence::Builder ala_builder;
            ala_builder.addEdge(lattice.firstOutEdge(
                                lattice.getVertexForRawCharIndex(0),
                                rawMask));
            ala_builder.addEdge(lattice.firstOutEdge(
                                lattice.getVertexForRawCharIndex(1),
                                rawMask));
            ala_builder.addEdge(lattice.firstOutEdge(
                                lattice.getVertexForRawCharIndex(2),
                                rawMask));

            lattice.addEdge(pre_ala, post_ala, word_token, token_tag, ala_builder.build());
        }

        {
            Lattice::EdgeSequence::Builder first_blank_builder;
            first_blank_builder.addEdge(lattice.firstOutEdge(
                                            lattice.getVertexForRawCharIndex(3),
                                            rawMask));
            lattice.addEdge(post_ala, pre_ma, blank_token, token_tag, first_blank_builder.build());
        }

        {
            Lattice::EdgeSequence::Builder ma_builder;
            ma_builder.addEdge(lattice.firstOutEdge(
                                lattice.getVertexForRawCharIndex(4),
                                rawMask));
            ma_builder.addEdge(lattice.firstOutEdge(
                                lattice.getVertexForRawCharIndex(5),
                                rawMask));
            lattice.addEdge(pre_ma, post_ma, word_token, token_tag, ma_builder.build());
        }

        {
            Lattice::EdgeSequence::Builder second_blank_builder;
            second_blank_builder.addEdge(lattice.firstOutEdge(
                                lattice.getVertexForRawCharIndex(6),
                                rawMask));
            lattice.addEdge(post_ma, pre_slonia, blank_token, token_tag, second_blank_builder.build());
        }

        {
            Lattice::EdgeSequence::Builder slonia_builder;
            slonia_builder.addEdge(lattice.firstOutEdge(
                                lattice.getVertexForRawCharIndex(7),
                                rawMask));
            slonia_builder.addEdge(lattice.firstOutEdge(
                                lattice.getVertexForRawCharIndex(8),
                                rawMask));
            slonia_builder.addEdge(lattice.firstOutEdge(
                                lattice.getVertexForRawCharIndex(10),
                                rawMask));
            slonia_builder.addEdge(lattice.firstOutEdge(
                                lattice.getVertexForRawCharIndex(11),
                                rawMask));
            slonia_builder.addEdge(lattice.firstOutEdge(
                                lattice.getVertexForRawCharIndex(12),
                                rawMask));
            slonia_builder.addEdge(lattice.firstOutEdge(
                                lattice.getVertexForRawCharIndex(13),
                                rawMask));
            lattice.addEdge(pre_slonia, post_slonia, word_token, token_tag, slonia_builder.build());
        }

    }


    void prepareAdvancedLattice_(Lattice & lattice) {

        lattice.appendString("Ala ma&nbsp;<b>kta</b>.");
        lattice.addSymbols(
            lattice.getFirstVertex(),
            lattice.getVertexForRawCharIndex(6)
        );
        lattice.addSymbols(
            lattice.getVertexForRawCharIndex(15),
            lattice.getVertexForRawCharIndex(18)
        );
        lattice.addSymbols(
            lattice.getVertexForRawCharIndex(22),
            lattice.getVertexForRawCharIndex(23)
        );


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


        Lattice::EdgeSequence::Builder alaBuilder;
        alaBuilder.addEdge(lattice.firstOutEdge(
                               lattice.getVertexForRawCharIndex(0),
                               rawMask));
        alaBuilder.addEdge(lattice.firstOutEdge(
                               lattice.getVertexForRawCharIndex(1),
                               rawMask));
        alaBuilder.addEdge(lattice.firstOutEdge(
                               lattice.getVertexForRawCharIndex(2),
                               rawMask));

        Lattice::EdgeDescriptor edgeAla
            = lattice.addEdge(preAla, postAla, aiAla, tokenTag, alaBuilder.build());


        std::list<std::string> lemmaTagsetStr;
        lemmaTagsetStr.push_back("lemma");
        lemmaTagsetStr.push_back("poleng-tagset");
        LayerTagCollection
            lemmaTagsetTag = lattice.getLayerTagManager().createTagCollection(lemmaTagsetStr);

        AnnotationItem aiAlaLemma("Ala");
        lattice.getAnnotationItemManager().setValue(aiAlaLemma, "pos", "R:4");
        lattice.getAnnotationItemManager().setValue(aiAlaLemma, "morpho", "ŻMP");

        Lattice::EdgeSequence::Builder alaLemmaBuilder;
        alaLemmaBuilder.addEdge(edgeAla);

        Lattice::EdgeDescriptor edgeAlaLemma
            = lattice.addEdge(preAla, postAla, aiAlaLemma, lemmaTagsetTag, alaLemmaBuilder.build());


        std::list<std::string> parseGobioStr;
        parseGobioStr.push_back("parse");
        parseGobioStr.push_back("gobio");
        LayerTagCollection
            parseGobioTag = lattice.getLayerTagManager().createTagCollection(parseGobioStr);

        AnnotationItem aiRzeczownik("rzeczownik");
        lattice.getAnnotationItemManager().setValue(aiRzeczownik, "R", "4");
        lattice.getAnnotationItemManager().setValue(aiRzeczownik, "L", "1");
        lattice.getAnnotationItemManager().setValue(aiRzeczownik, "P", "mian");

        Lattice::EdgeSequence::Builder rzeczownikBuilder;
        rzeczownikBuilder.addEdge(edgeAlaLemma);

        Lattice::EdgeDescriptor edgeRzeczownik
            = lattice.addEdge(preAla, postAla, aiRzeczownik, parseGobioTag, rzeczownikBuilder.build());


        Lattice::VertexDescriptor preMa = lattice.getVertexForRawCharIndex(4);

        AnnotationItem aiBlank("' '");
        lattice.getAnnotationItemManager().setValue(aiBlank, "type", "blank");

        Lattice::EdgeSequence::Builder blankBuilder;
        blankBuilder.addEdge(lattice.firstOutEdge(
                                 lattice.getVertexForRawCharIndex(3),
                                 rawMask));

        Lattice::EdgeDescriptor edgeBlank
            = lattice.addEdge(postAla, preMa, aiBlank, tokenTag, blankBuilder.build());


        Lattice::VertexDescriptor postMa = lattice.getVertexForRawCharIndex(6);

        AnnotationItem aiMa("'ma'");
        lattice.getAnnotationItemManager().setValue(aiMa, "type", "word");

        Lattice::EdgeSequence::Builder maBuilder;
        maBuilder.addEdge(lattice.firstOutEdge(
                              lattice.getVertexForRawCharIndex(4),
                              rawMask));
        maBuilder.addEdge(lattice.firstOutEdge(
                              lattice.getVertexForRawCharIndex(5),
                              rawMask));
        Lattice::EdgeDescriptor edgeMa
            = lattice.addEdge(preMa, postMa, aiMa, tokenTag, maBuilder.build());


        Lattice::VertexDescriptor preMarkup = lattice.getVertexForRawCharIndex(12);

        AnnotationItem aiNbsp("' ");

        lattice.addEdge(postMa, preMarkup, aiNbsp, rawTag);

        Lattice::EdgeSequence::Builder nbspBlankBuilder;
        nbspBlankBuilder.addEdge(lattice.firstOutEdge(
                                 lattice.getVertexForRawCharIndex(6),
                                 rawMask));

        Lattice::EdgeDescriptor edgeNbsp
            = lattice.addEdge(postMa, preMarkup, aiBlank, tokenTag, nbspBlankBuilder.build());


        Lattice::VertexDescriptor preKota = lattice.getVertexForRawCharIndex(15);

        AnnotationItem aiOpen("open");

        std::list<std::string> markupHtmlStr;
        markupHtmlStr.push_back("markup");
        markupHtmlStr.push_back("html");
        LayerTagCollection
            markupHtmlTag = lattice.getLayerTagManager().createTagCollection(markupHtmlStr);

        Lattice::EdgeDescriptor edgeMarkupOpen
            = lattice.addEdge(preMarkup, preKota, aiOpen, markupHtmlTag);


        Lattice::VertexDescriptor postKota = lattice.getVertexForRawCharIndex(18);

        AnnotationItem aiKta("'kta'", "kta");
        lattice.getAnnotationItemManager().setValue(aiKta, "type", "word");

        {
            Lattice::EdgeSequence::Builder ktaBuilder;
            ktaBuilder.addEdge(lattice.firstOutEdge(
                                  lattice.getVertexForRawCharIndex(15),
                                  rawMask));
            ktaBuilder.addEdge(lattice.firstOutEdge(
                                  lattice.getVertexForRawCharIndex(16),
                                  rawMask));
            ktaBuilder.addEdge(lattice.firstOutEdge(
                                  lattice.getVertexForRawCharIndex(17),
                                  rawMask));
            lattice.addEdge(preKota, postKota, aiKta, tokenTag, ktaBuilder.build());
        }


        AnnotationItem aiK("'k", "k");
        Lattice::VertexDescriptor vdKO = lattice.addLooseVertex();
        Lattice::EdgeDescriptor edgeCorrectedK = lattice.addEdge(preKota, vdKO, aiK, rawTag);


        AnnotationItem aiO("'o", "o");
        Lattice::VertexDescriptor midKota = lattice.getVertexForRawCharIndex(16);
        Lattice::EdgeDescriptor edgeCorrectedO = lattice.addEdge(vdKO, midKota, aiO, rawTag);


        AnnotationItem aiKota("'kota'", "kota");
        lattice.getAnnotationItemManager().setValue(aiKota, "type", "word");

        std::list<std::string> tokenCorrectorStr;
        tokenCorrectorStr.push_back("token");
        tokenCorrectorStr.push_back("corrector");
        LayerTagCollection
            tokenCorrectorTag = lattice.getLayerTagManager().createTagCollection(tokenCorrectorStr);

        Lattice::EdgeSequence::Builder kotaBuilder;
        kotaBuilder.addEdge(edgeCorrectedK);
        kotaBuilder.addEdge(edgeCorrectedO);
        kotaBuilder.addEdge(lattice.firstOutEdge(
                              lattice.getVertexForRawCharIndex(16),
                              rawMask));
        kotaBuilder.addEdge(lattice.firstOutEdge(
                              lattice.getVertexForRawCharIndex(17),
                              rawMask));
        Lattice::EdgeDescriptor edgeKota
            = lattice.addEdge(preKota, postKota, aiKota, tokenCorrectorTag, kotaBuilder.build());


        AnnotationItem aiFrazaRzecz("fraza_rzeczownikowa");
        lattice.getAnnotationItemManager().setValue(aiFrazaRzecz, "R", "2");
        lattice.getAnnotationItemManager().setValue(aiFrazaRzecz, "L", "1");
        lattice.getAnnotationItemManager().setValue(aiFrazaRzecz, "P", "dop");

        Lattice::EdgeSequence::Builder frazaRzeczBuilder;
        frazaRzeczBuilder.addEdge(edgeKota);

        Lattice::EdgeDescriptor edgeFrazaRzecz
            = lattice.addEdge(preKota, postKota, aiFrazaRzecz, parseGobioTag, frazaRzeczBuilder.build());


        Lattice::VertexDescriptor postMarkup = lattice.getVertexForRawCharIndex(22);

        AnnotationItem aiClose("close");

        Lattice::EdgeDescriptor edgeMarkupClose
            = lattice.addEdge(postKota, postMarkup, aiClose, markupHtmlTag);


        Lattice::EdgeSequence::Builder frazaRzecz2Builder;
        frazaRzecz2Builder.addEdge(edgeMarkupOpen);
        frazaRzecz2Builder.addEdge(edgeFrazaRzecz);
        frazaRzecz2Builder.addEdge(edgeMarkupClose);

        Lattice::EdgeDescriptor edgeFrazaRzecz2
            = lattice.addEdge(preMarkup, postMarkup, aiFrazaRzecz, parseGobioTag, frazaRzecz2Builder.build());


        AnnotationItem aiFrazaCzas("fraza_czasownikowa");
        lattice.getAnnotationItemManager().setValue(aiFrazaCzas, "C", "teraźniejszy");
        lattice.getAnnotationItemManager().setValue(aiFrazaCzas, "O", "3");
        lattice.getAnnotationItemManager().setValue(aiFrazaCzas, "L", "1");

        Lattice::EdgeSequence::Builder frazaCzasBuilder;
        frazaCzasBuilder.addEdge(edgeMa);
        frazaCzasBuilder.addEdge(edgeNbsp);
        frazaCzasBuilder.addEdge(edgeFrazaRzecz2);

        Lattice::EdgeDescriptor edgeFrazaCzas
            = lattice.addEdge(preMa, postMarkup, aiFrazaCzas, parseGobioTag, frazaCzasBuilder.build());


        AnnotationItem aiPelnaFrCzas("pełna_fraza_czasownikowa");
        lattice.getAnnotationItemManager().setValue(aiPelnaFrCzas, "C", "teraźniejszy");

        Lattice::EdgeSequence::Builder pelnaFrCzasBuilder;
        pelnaFrCzasBuilder.addEdge(edgeRzeczownik);
        pelnaFrCzasBuilder.addEdge(edgeBlank);
        pelnaFrCzasBuilder.addEdge(edgeFrazaCzas);

        Lattice::EdgeDescriptor edgePelnaFrCzas
            = lattice.addEdge(preAla, postMarkup, aiPelnaFrCzas, parseGobioTag, pelnaFrCzasBuilder.build(), -0.342);


        Lattice::VertexDescriptor postStop = lattice.getVertexForRawCharIndex(23);

        AnnotationItem aiStop("'.'");
        lattice.getAnnotationItemManager().setValue(aiStop, "type", "punct");

        Lattice::EdgeSequence::Builder stopBuilder;
        stopBuilder.addEdge(lattice.firstOutEdge(
                              lattice.getVertexForRawCharIndex(22),
                              rawMask));
        Lattice::EdgeDescriptor edgeStop
            = lattice.addEdge(postMarkup, postStop, aiStop, tokenTag, stopBuilder.build());


        LayerTagCollection
            splitterTag = lattice.getLayerTagManager().createSingletonTagCollection("splitter");

        AnnotationItem aiSentence("sentence");

        lattice.addEdge(preAla, postStop, aiSentence, splitterTag);


        AnnotationItem aiZdanie("zdanie");

        Lattice::EdgeSequence::Builder zdanieBuilder;
        zdanieBuilder.addEdge(edgePelnaFrCzas);
        zdanieBuilder.addEdge(edgeStop);

        lattice.addEdge(preAla, postStop, aiZdanie, parseGobioTag, zdanieBuilder.build());

    }

};
