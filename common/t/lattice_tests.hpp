
#include "lattice.hpp"
#include "annotation_item.hpp"

#include "by_spaces_cutter.hpp"
#include "fake_lemmatizer.hpp"
#include "lemmatizer_annotator.hpp"

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
            lattice.addEdge(post_ma, pre_kota, blank_token, token_tag, second_blank_builder.build());
        }

        {
            Lattice::EdgeSequence::Builder kota_builder;
            kota_builder.addEdge(lattice.firstOutEdge(
                                lattice.getVertexForRawCharIndex(7),
                                rawMask));
            kota_builder.addEdge(lattice.firstOutEdge(
                                lattice.getVertexForRawCharIndex(8),
                                rawMask));
            kota_builder.addEdge(lattice.firstOutEdge(
                                lattice.getVertexForRawCharIndex(9),
                                rawMask));
            kota_builder.addEdge(lattice.firstOutEdge(
                                lattice.getVertexForRawCharIndex(10),
                                rawMask));
            lattice.addEdge(pre_kota, post_kota, word_token, token_tag, kota_builder.build());
        }

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

    void testVertexIterator() {
        Lattice lattice;
        lattice.appendStringWithSymbols("żeś");

        Lattice::VertexIterator iter(lattice);

        size_t count = 0;
        while (iter.hasNext()) {
            iter.next();
            ++count;
        }

        TS_ASSERT_EQUALS(count, 4U);
    }

    void test_get_path() {
        Lattice lattice;
        lattice.appendStringWithSymbols("ćma zielona");
        lattice.appendString("<br>");
        lattice.appendStringWithSymbols("mucha");

        LayerTagMask symbolMask = lattice.getLayerTagManager().getMask("symbol");

        Lattice::VertexDescriptor vertex = lattice.getFirstVertex();

        Lattice::EdgeSequence sequence = lattice.getPath(vertex, symbolMask);

        Lattice::InOutEdgesIterator iter = lattice.outEdges(vertex, symbolMask);
        TS_ASSERT(!iter.hasNext());

        TS_ASSERT_EQUALS(sequence.size(), 11U);
    }

    void test_cutter() {
        Lattice lattice;
        lattice.appendStringWithSymbols("szybki zielony rower");

        BySpacesCutter cutter;

        LayerTagMask symbolMask = lattice.getLayerTagManager().getMask("symbol");

        lattice.runCutter(cutter, symbolMask);

        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask("token");

        Lattice::EdgesSortedBySourceIterator tokenIter = lattice.edgesSortedBySource(tokenMask);

        {
            TS_ASSERT(tokenIter.hasNext());
            AnnotationItem item = lattice.getEdgeAnnotationItem(tokenIter.next());
            TS_ASSERT_EQUALS(item.getText(), "szybki");
            TS_ASSERT_EQUALS(item.getCategory(), "word");
        }

        {
            TS_ASSERT(tokenIter.hasNext());
            AnnotationItem item = lattice.getEdgeAnnotationItem(tokenIter.next());
            TS_ASSERT_EQUALS(item.getText(), " ");
            TS_ASSERT_EQUALS(item.getCategory(), "blank");
        }

        {
            TS_ASSERT(tokenIter.hasNext());
            AnnotationItem item = lattice.getEdgeAnnotationItem(tokenIter.next());
            TS_ASSERT_EQUALS(item.getText(), "zielony");
            TS_ASSERT_EQUALS(item.getCategory(), "word");
        }

        {
            TS_ASSERT(tokenIter.hasNext());
            AnnotationItem item = lattice.getEdgeAnnotationItem(tokenIter.next());
            TS_ASSERT_EQUALS(item.getText(), " ");
            TS_ASSERT_EQUALS(item.getCategory(), "blank");
        }

        {
            TS_ASSERT(tokenIter.hasNext());
            AnnotationItem item = lattice.getEdgeAnnotationItem(tokenIter.next());
            TS_ASSERT_EQUALS(item.getText(), "rower");
            TS_ASSERT_EQUALS(item.getCategory(), "word");
        }

        TS_ASSERT(!tokenIter.hasNext());
    }

    void test_cutter_on_no_symbols() {
        Lattice lattice("<a>");

        BySpacesCutter cutter;

        LayerTagMask symbolMask = lattice.getLayerTagManager().getMask("symbol");

        lattice.runCutter(cutter, symbolMask);

        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask("token");

        Lattice::EdgesSortedBySourceIterator tokenIter = lattice.edgesSortedBySource(tokenMask);

        TS_ASSERT(!tokenIter.hasNext());
    }

    void test_lemmatizer() {
        Lattice lattice;
        lattice.appendStringWithSymbols("prowokacjami");

        BySpacesCutter cutter;

        LayerTagMask symbolMask = lattice.getLayerTagManager().getMask("symbol");

        lattice.runCutter(cutter, symbolMask);

        boost::program_options::variables_map noOptions;
        LemmatizerAnnotator<FakeLemmatizer> annotator(noOptions);

        annotator.annotate(lattice);

        // now checking

        {
            LayerTagMask lemmaMask_ = lattice.getLayerTagManager().getMask("lexeme");
            Lattice::EdgesSortedByTargetIterator lemmaIter = lattice.edgesSortedByTarget(lemmaMask_);

            TS_ASSERT(lemmaIter.hasNext());
            Lattice::EdgeDescriptor prowokacjamiLemma = lemmaIter.next();
            AnnotationItem prowokacjamiItem = lattice.getEdgeAnnotationItem(prowokacjamiLemma);

            TS_ASSERT_EQUALS(prowokacjamiItem.getCategory(), "R");
            TS_ASSERT_EQUALS(prowokacjamiItem.getText(), "prowokacja_R");

            TS_ASSERT_EQUALS(
                lattice.getVertexRawCharIndex(lattice.getEdgeSource(prowokacjamiLemma)),
                0U);

            TS_ASSERT_EQUALS(
                lattice.getVertexRawCharIndex(lattice.getEdgeTarget(prowokacjamiLemma)),
                12U);

            TS_ASSERT(!lemmaIter.hasNext());
        }

        {
            LayerTagMask formMask_ = lattice.getLayerTagManager().getMask("form");
            Lattice::EdgesSortedByTargetIterator formIter = lattice.edgesSortedByTarget(formMask_);

            TS_ASSERT(formIter.hasNext());
            Lattice::EdgeDescriptor prowokacjamiForm = formIter.next();
            AnnotationItem prowokacjamiItem = lattice.getEdgeAnnotationItem(prowokacjamiForm);

            TS_ASSERT_EQUALS(prowokacjamiItem.getCategory(), "R");
            TS_ASSERT_EQUALS(prowokacjamiItem.getText(), "prowokacja_R");

            TS_ASSERT(!formIter.hasNext());
        }
    }

    void testVariantEdges() {
        //preparing lattice
        Lattice lattice("ananas");
        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
        LayerTagCollection raw_tag
            = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
        LayerTagCollection token_tag
            = lattice.getLayerTagManager().createSingletonTagCollection("token");
        LayerTagCollection lemma_tag
            = lattice.getLayerTagManager().createSingletonTagCollection("lemma");
        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
        LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);

        Lattice::VertexDescriptor pre_ananas = lattice.getFirstVertex();
        Lattice::VertexDescriptor post_ananas = lattice.getVertexForRawCharIndex(6);

        AnnotationItem word_token("word");
        AnnotationItem blank_token("blank");

        Lattice::EdgeSequence::Builder ananas_builder;
        for (int i = 0; i < 6; i ++) {
            ananas_builder.addEdge(lattice.firstOutEdge(
                        lattice.getVertexForRawCharIndex(i),
                        rawMask
                        ));
        }
        lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build());

        Lattice::EdgeSequence::Builder ananas_lemma_builder;
        ananas_lemma_builder.addEdge(lattice.firstOutEdge(lattice.getVertexForRawCharIndex(0), tokenMask));
        AnnotationItem ai_ananas_sg("ananas"); // singular masculinie noun variant
        lattice.getAnnotationItemManager().setValue(ai_ananas_sg, "base", "ananas");
        lattice.getAnnotationItemManager().setValue(ai_ananas_sg, "morphology", "subst:sg:m");
        lattice.getAnnotationItemManager().setValue(ai_ananas_sg, "discard", "0");
        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_sg, lemma_tag, ananas_lemma_builder.build());

        AnnotationItem ai_ananas_pl("ananas"); // plural masculine noun variant
        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "base", "ananas");
        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "morphology", "subst:pl:m");
        lattice.getAnnotationItemManager().setValue(ai_ananas_pl, "discard", "0");
        lattice.addEdge(pre_ananas, post_ananas, ai_ananas_pl, lemma_tag, ananas_lemma_builder.build());

        Lattice::EdgeDescriptor edge;
        Lattice::EdgesSortedBySourceIterator tokenIter = lattice.edgesSortedBySource(lemmaMask);
        TS_ASSERT(tokenIter.hasNext());
        edge = tokenIter.next();
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_ananas_sg.getCategory()
        );
        std::list< std::pair<std::string, std::string> > av
            = lattice.getAnnotationItemManager().getValues(
                    lattice.getEdgeAnnotationItem(edge)
                    );
        std::list< std::pair<std::string, std::string> >::iterator avi = av.begin();
        TS_ASSERT_EQUALS((*avi).first, "base");
        TS_ASSERT_EQUALS((*avi).second, "ananas");
        ++avi;
        TS_ASSERT_EQUALS((*avi).first, "morphology");
        TS_ASSERT_EQUALS((*avi).second, "subst:sg:m");
        ++avi;
        TS_ASSERT(avi != av.end());
        TS_ASSERT_EQUALS((*avi).first, "discard");
        TS_ASSERT_EQUALS((*avi).second, "0");
        ++avi;
        TS_ASSERT(avi == av.end());
        TS_ASSERT(tokenIter.hasNext());
        edge = tokenIter.next();
        TS_ASSERT_EQUALS(
            lattice.getEdgeAnnotationItem(edge).getCategory(),
            ai_ananas_pl.getCategory()
        );
        av = lattice.getAnnotationItemManager().getValues(
                    lattice.getEdgeAnnotationItem(edge)
                    );
        avi = av.begin();
        TS_ASSERT_EQUALS((*avi).first, "base");
        TS_ASSERT_EQUALS((*avi).second, "ananas");
        ++avi;
        TS_ASSERT_EQUALS((*avi).first, "morphology");
        TS_ASSERT_EQUALS((*avi).second, "subst:pl:m");
        ++avi;
        TS_ASSERT(avi != av.end());
        TS_ASSERT_EQUALS((*avi).first, "discard");
        TS_ASSERT_EQUALS((*avi).second, "0");
        ++avi;
        TS_ASSERT(avi == av.end());
        TS_ASSERT(!tokenIter.hasNext());

    }

    void testEdgesLayerTags() {
        //preparing lattice
        Lattice lattice("ananas");
        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
        LayerTagCollection raw_tag
            = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
        LayerTagCollection token_tag
            = lattice.getLayerTagManager().createSingletonTagCollection("token");
        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);

        Lattice::VertexDescriptor pre_ananas = lattice.getFirstVertex();
        Lattice::VertexDescriptor post_ananas = lattice.getVertexForRawCharIndex(6);

        AnnotationItem word_token("word");
        AnnotationItem blank_token("blank");

        Lattice::EdgeSequence::Builder ananas_builder;
        for (int i = 0; i < 6; i ++) {
            ananas_builder.addEdge(lattice.firstOutEdge(
                        lattice.getVertexForRawCharIndex(i),
                        rawMask
                        ));
        }
        lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build());

        Lattice::EdgeDescriptor edge;
        Lattice::EdgesSortedBySourceIterator rawIter = lattice.edgesSortedBySource(rawMask);
        TS_ASSERT(rawIter.hasNext());
        edge = rawIter.next();
        std::list<std::string> tagNames
            = lattice.getLayerTagManager().getTagNames(lattice.getEdgeLayerTags(edge));
        TS_ASSERT_EQUALS(tagNames.size(), (size_t) 1);
        std::list<std::string>::iterator tni = tagNames.begin();
        TS_ASSERT_EQUALS(*tni, "symbol");
        ++tni;
        TS_ASSERT(tni == tagNames.end());

        Lattice::EdgesSortedBySourceIterator tokenIter = lattice.edgesSortedBySource(tokenMask);
        TS_ASSERT(tokenIter.hasNext());
        edge = tokenIter.next();
        tagNames = lattice.getLayerTagManager().getTagNames(lattice.getEdgeLayerTags(edge));
        TS_ASSERT_EQUALS(tagNames.size(), (size_t) 1);
        tni = tagNames.begin();
        TS_ASSERT_EQUALS(*tni, "token");
        ++tni;
        TS_ASSERT(tni == tagNames.end());
    }

    void testEdgesTagsCombining() {
        //preparing lattice
        Lattice lattice("ananas");
        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
        LayerTagCollection raw_tag
            = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
        LayerTagCollection token_tag
            = lattice.getLayerTagManager().createSingletonTagCollection("token");
        LayerTagCollection lemma_tag
            = lattice.getLayerTagManager().createSingletonTagCollection("lemma");
        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);
        LayerTagMask lemmaMask = lattice.getLayerTagManager().getMask(lemma_tag);
        LayerTagMask tokenOrLemmaMask = createUnion(tokenMask, lemmaMask);

        Lattice::VertexDescriptor pre_ananas = lattice.getFirstVertex();
        Lattice::VertexDescriptor post_ananas = lattice.getVertexForRawCharIndex(6);

        AnnotationItem word_token("word");
        AnnotationItem blank_token("blank");

        Lattice::EdgeSequence::Builder ananas_builder;
        for (int i = 0; i < 6; i ++) {
            ananas_builder.addEdge(lattice.firstOutEdge(
                        lattice.getVertexForRawCharIndex(i),
                        rawMask
                        ));
        }
        lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build());
        lattice.addEdge(pre_ananas, post_ananas, word_token, lemma_tag, ananas_builder.build());

        Lattice::EdgeDescriptor edge;
        Lattice::EdgesSortedBySourceIterator tokenOrLemmaIter = lattice.edgesSortedBySource(tokenOrLemmaMask);
        TS_ASSERT(tokenOrLemmaIter.hasNext());
        edge = tokenOrLemmaIter.next();
        std::list<std::string> tagNames
            = lattice.getLayerTagManager().getTagNames(lattice.getEdgeLayerTags(edge));
        TS_ASSERT_EQUALS(tagNames.size(), (size_t) 2);
        std::list<std::string>::iterator tni = tagNames.begin();
        TS_ASSERT_EQUALS(*tni, "lemma");
        ++tni;
        TS_ASSERT(tni != tagNames.end());
        TS_ASSERT_EQUALS(*tni, "token");
    }

    void testEdgesScoresCombining() {
        //preparing lattice
        Lattice lattice("ananas");
        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
        LayerTagCollection raw_tag
            = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
        LayerTagCollection token_tag
            = lattice.getLayerTagManager().createSingletonTagCollection("token");
        LayerTagMask rawMask = lattice.getLayerTagManager().getMask(raw_tag);
        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);

        Lattice::VertexDescriptor pre_ananas = lattice.getFirstVertex();
        Lattice::VertexDescriptor post_ananas = lattice.getVertexForRawCharIndex(6);

        AnnotationItem word_token("word");
        AnnotationItem blank_token("blank");

        Lattice::EdgeSequence::Builder ananas_builder;
        for (int i = 0; i < 6; i ++) {
            ananas_builder.addEdge(lattice.firstOutEdge(
                        lattice.getVertexForRawCharIndex(i),
                        rawMask
                        ));
        }

        lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build(), -8.0);
        Lattice::EdgesSortedBySourceIterator tokenIter1(lattice.edgesSortedBySource(tokenMask));
        TS_ASSERT(tokenIter1.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeScore(tokenIter1.next()), -8.0);

        lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build(), -2.0);
        Lattice::EdgesSortedBySourceIterator tokenIter2(lattice.edgesSortedBySource(tokenMask));
        TS_ASSERT(tokenIter2.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeScore(tokenIter2.next()), -2.0);

        lattice.addEdge(pre_ananas, post_ananas, word_token, token_tag, ananas_builder.build(), -4.0);
        Lattice::EdgesSortedBySourceIterator tokenIter3(lattice.edgesSortedBySource(tokenMask));
        TS_ASSERT(tokenIter3.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeScore(tokenIter3.next()), -2.0);
    }

    void testLooseVertices() {
        //preparing lattice
        Lattice lattice("abc");
        lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());
        LayerTagCollection token_tag
            = lattice.getLayerTagManager().createSingletonTagCollection("token");
        LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(token_tag);

        Lattice::VertexDescriptor vertexPre = lattice.getVertexForRawCharIndex(1);
        // Lattice::VertexDescriptor vertexPost = lattice.getVertexForRawCharIndex(2);
        Lattice::VertexDescriptor vertexLoose = lattice.addLooseVertex();

        AnnotationItem tokenX("x");
        AnnotationItem tokenY("y");

        lattice.addEdge(vertexPre, vertexLoose, tokenX, token_tag);

        Lattice::InOutEdgesIterator eiPreOut = lattice.outEdges(vertexPre, tokenMask);
        TS_ASSERT(eiPreOut.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(eiPreOut.next()).getCategory(), "x");

        Lattice::InOutEdgesIterator eiLooseIn = lattice.inEdges(vertexLoose, tokenMask);
        TS_ASSERT(eiLooseIn.hasNext());
        TS_ASSERT_EQUALS(lattice.getEdgeAnnotationItem(eiLooseIn.next()).getCategory(), "x");
    }


};
