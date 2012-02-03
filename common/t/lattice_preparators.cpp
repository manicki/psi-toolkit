#include "lattice_preparators.hpp"


void lattice_preparators::prepareSimpleLattice(Lattice & lattice) {

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
        Lattice::EdgeSequence::Builder ala_builder(lattice);
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
        Lattice::EdgeSequence::Builder first_blank_builder(lattice);
        first_blank_builder.addEdge(lattice.firstOutEdge(
                                        lattice.getVertexForRawCharIndex(3),
                                        rawMask));
        lattice.addEdge(post_ala, pre_ma, blank_token, token_tag, first_blank_builder.build());
    }

    {
        Lattice::EdgeSequence::Builder ma_builder(lattice);
        ma_builder.addEdge(lattice.firstOutEdge(
                               lattice.getVertexForRawCharIndex(4),
                               rawMask));
        ma_builder.addEdge(lattice.firstOutEdge(
                               lattice.getVertexForRawCharIndex(5),
                               rawMask));
        lattice.addEdge(pre_ma, post_ma, word_token, token_tag, ma_builder.build());
    }

    {
        Lattice::EdgeSequence::Builder second_blank_builder(lattice);
        second_blank_builder.addEdge(lattice.firstOutEdge(
                                         lattice.getVertexForRawCharIndex(6),
                                         rawMask));
        lattice.addEdge(post_ma, pre_slonia, blank_token, token_tag, second_blank_builder.build());
    }

    {
        Lattice::EdgeSequence::Builder slonia_builder(lattice);
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


void lattice_preparators::prepareAdvancedLattice(Lattice & lattice) {

    lattice.appendString("Ala ma&nbsp;<b>kta</b>.");
    const std::string & ltext = lattice.getAllText();
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

    AnnotationItem aiAla("'Ala'", StringFrag(ltext, 0, 3));
    lattice.getAnnotationItemManager().setValue(aiAla, "type", "word");


    Lattice::EdgeSequence::Builder alaBuilder(lattice);
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

    Lattice::EdgeSequence::Builder alaLemmaBuilder(lattice);
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

    Lattice::EdgeSequence::Builder rzeczownikBuilder(lattice);
    rzeczownikBuilder.addEdge(edgeAlaLemma);

    Lattice::EdgeDescriptor edgeRzeczownik
        = lattice.addEdge(preAla, postAla, aiRzeczownik, parseGobioTag, rzeczownikBuilder.build());


    Lattice::VertexDescriptor preMa = lattice.getVertexForRawCharIndex(4);

    AnnotationItem aiBlank("' '", StringFrag(ltext, 3, 1));
    lattice.getAnnotationItemManager().setValue(aiBlank, "type", "blank");

    Lattice::EdgeSequence::Builder blankBuilder(lattice);
    blankBuilder.addEdge(lattice.firstOutEdge(
                             lattice.getVertexForRawCharIndex(3),
                             rawMask));

    Lattice::EdgeDescriptor edgeBlank
        = lattice.addEdge(postAla, preMa, aiBlank, tokenTag, blankBuilder.build());


    Lattice::VertexDescriptor postMa = lattice.getVertexForRawCharIndex(6);

    AnnotationItem aiMa("'ma'", StringFrag(ltext, 4, 2));
    lattice.getAnnotationItemManager().setValue(aiMa, "type", "word");

    Lattice::EdgeSequence::Builder maBuilder(lattice);
    maBuilder.addEdge(lattice.firstOutEdge(
                          lattice.getVertexForRawCharIndex(4),
                          rawMask));
    maBuilder.addEdge(lattice.firstOutEdge(
                          lattice.getVertexForRawCharIndex(5),
                          rawMask));
    Lattice::EdgeDescriptor edgeMa
        = lattice.addEdge(preMa, postMa, aiMa, tokenTag, maBuilder.build());


    Lattice::VertexDescriptor preMarkup = lattice.getVertexForRawCharIndex(12);

    AnnotationItem aiNbsp("' ", StringFrag(" "));

    lattice.addEdge(postMa, preMarkup, aiNbsp, rawTag);

    Lattice::EdgeSequence::Builder nbspBlankBuilder(lattice);
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

    AnnotationItem aiKta("'kta'", StringFrag(ltext, 15, 3));
    lattice.getAnnotationItemManager().setValue(aiKta, "type", "word");

    {
        Lattice::EdgeSequence::Builder ktaBuilder(lattice);
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


    AnnotationItem aiK("'k", StringFrag(ltext, 15, 1));
    Lattice::VertexDescriptor vdKO = lattice.addLooseVertex();
    Lattice::EdgeDescriptor edgeCorrectedK = lattice.addEdge(preKota, vdKO, aiK, rawTag);


    AnnotationItem aiO("'o", StringFrag("o"));
    Lattice::VertexDescriptor midKota = lattice.getVertexForRawCharIndex(16);
    Lattice::EdgeDescriptor edgeCorrectedO = lattice.addEdge(vdKO, midKota, aiO, rawTag);


    AnnotationItem aiKota("'kota'", StringFrag("kota"));
    lattice.getAnnotationItemManager().setValue(aiKota, "type", "word");

    std::list<std::string> tokenCorrectorStr;
    tokenCorrectorStr.push_back("token");
    tokenCorrectorStr.push_back("corrector");
    LayerTagCollection
        tokenCorrectorTag = lattice.getLayerTagManager().createTagCollection(tokenCorrectorStr);

    Lattice::EdgeSequence::Builder kotaBuilder(lattice);
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

    Lattice::EdgeSequence::Builder frazaRzeczBuilder(lattice);
    frazaRzeczBuilder.addEdge(edgeKota);

    Lattice::EdgeDescriptor edgeFrazaRzecz
        = lattice.addEdge(preKota, postKota,
                          aiFrazaRzecz, parseGobioTag, frazaRzeczBuilder.build());


    Lattice::VertexDescriptor postMarkup = lattice.getVertexForRawCharIndex(22);

    AnnotationItem aiClose("close");

    Lattice::EdgeDescriptor edgeMarkupClose
        = lattice.addEdge(postKota, postMarkup, aiClose, markupHtmlTag);


    Lattice::EdgeSequence::Builder frazaRzecz2Builder(lattice);
    frazaRzecz2Builder.addEdge(edgeMarkupOpen);
    frazaRzecz2Builder.addEdge(edgeFrazaRzecz);
    frazaRzecz2Builder.addEdge(edgeMarkupClose);

    Lattice::EdgeDescriptor edgeFrazaRzecz2
        = lattice.addEdge(preMarkup, postMarkup,
                          aiFrazaRzecz, parseGobioTag, frazaRzecz2Builder.build());


    AnnotationItem aiFrazaCzas("fraza_czasownikowa");
    lattice.getAnnotationItemManager().setValue(aiFrazaCzas, "C", "teraźniejszy");
    lattice.getAnnotationItemManager().setValue(aiFrazaCzas, "O", "3");
    lattice.getAnnotationItemManager().setValue(aiFrazaCzas, "L", "1");

    Lattice::EdgeSequence::Builder frazaCzasBuilder(lattice);
    frazaCzasBuilder.addEdge(edgeMa);
    frazaCzasBuilder.addEdge(edgeNbsp);
    frazaCzasBuilder.addEdge(edgeFrazaRzecz2);

    Lattice::EdgeDescriptor edgeFrazaCzas
        = lattice.addEdge(preMa, postMarkup, aiFrazaCzas, parseGobioTag, frazaCzasBuilder.build());


    AnnotationItem aiPelnaFrCzas("pełna_fraza_czasownikowa");
    lattice.getAnnotationItemManager().setValue(aiPelnaFrCzas, "C", "teraźniejszy");

    Lattice::EdgeSequence::Builder pelnaFrCzasBuilder(lattice);
    pelnaFrCzasBuilder.addEdge(edgeRzeczownik);
    pelnaFrCzasBuilder.addEdge(edgeBlank);
    pelnaFrCzasBuilder.addEdge(edgeFrazaCzas);

    Lattice::EdgeDescriptor edgePelnaFrCzas
        = lattice.addEdge(
            preAla, postMarkup,
            aiPelnaFrCzas, parseGobioTag, pelnaFrCzasBuilder.build(), -0.342);

    Lattice::VertexDescriptor postStop = lattice.getVertexForRawCharIndex(23);

    AnnotationItem aiStop("'.'", StringFrag(ltext, 22, 1));
    lattice.getAnnotationItemManager().setValue(aiStop, "type", "punct");

    Lattice::EdgeSequence::Builder stopBuilder(lattice);
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

    Lattice::EdgeSequence::Builder zdanieBuilder(lattice);
    zdanieBuilder.addEdge(edgePelnaFrCzas);
    zdanieBuilder.addEdge(edgeStop);

    lattice.addEdge(preAla, postStop, aiZdanie, parseGobioTag, zdanieBuilder.build());

}


void lattice_preparators::prepareRegularLattice(Lattice & lattice) {

    std::string text(
        "Aa1Aa2Aa3Ab1Ab2Ab3Ac1Ac2Ac3Ba1Ba2Ba3Bb1Bb2Bb3Bc1Bc2Bc3Ca1Ca2Ca3Cb1Cb2Cb3Cc1Cc2Cc3"
    );
    lattice.appendString(text);
    lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());

    Lattice::VertexDescriptor vd[28];
    for (int i = 0; i <= 27; ++i) {
        vd[i] = lattice.getVertexForRawCharIndex(3*i);
    }

    LayerTagCollection
        tagRaw = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
    LayerTagCollection
        tagToken = lattice.getLayerTagManager().createSingletonTagCollection("token");
    LayerTagCollection
        tagLevel1 = lattice.getLayerTagManager().createSingletonTagCollection("level1");
    LayerTagCollection
        tagLevel2 = lattice.getLayerTagManager().createSingletonTagCollection("level2");

    AnnotationItem *ai[27];
    for (int i = 0; i < 27; ++i) {
        std::string aiText = text.substr(3*i, 3);
        ai[i] = new AnnotationItem("token", aiText);
        lattice.addEdge(vd[i], vd[i+1], *ai[i], tagToken);
        delete ai[i];
    }

    AnnotationItem *ai2[9];
    for (int i = 0; i < 9; ++i) {
        std::string aiText = text.substr(9*i, 9);
        ai2[i] = new AnnotationItem("level1", aiText);
        lattice.addEdge(vd[3*i], vd[3*i+3], *ai2[i], tagLevel1);
        delete ai2[i];
    }

    AnnotationItem *ai3[3];
    for (int i = 0; i < 3; ++i) {
        std::string aiText = text.substr(27*i, 27);
        ai3[i] = new AnnotationItem("level2", aiText);
        lattice.addEdge(vd[9*i], vd[9*i+9], *ai3[i], tagLevel2);
        delete ai3[i];
    }

}


void lattice_preparators::prepareLatticeWithOneSymbolTokens(Lattice & lattice, std::string text) {

    lattice.appendString(text);
    lattice.addSymbols(lattice.getFirstVertex(), lattice.getLastVertex());

    Lattice::VertexDescriptor * vd = new Lattice::VertexDescriptor[text.length() + 1];
    for (size_t i = 0; i <= text.length(); ++i) {
        vd[i] = lattice.getVertexForRawCharIndex(i);
    }

    LayerTagCollection
        tagRaw = lattice.getLayerTagManager().createSingletonTagCollection("symbol");
    LayerTagCollection
        tagToken = lattice.getLayerTagManager().createSingletonTagCollection("token");

    LayerTagMask maskRaw = lattice.getLayerTagManager().getMask(tagRaw);

    AnnotationItem ** ai = new AnnotationItem * [text.length()];
    for (size_t i = 0; i < text.length(); ++i) {
        std::string aiCategory = text.substr(i, 1);
        StringFrag aiText(aiCategory);
        ai[i] = new AnnotationItem(aiCategory, aiText);
        Lattice::EdgeSequence::Builder builder(lattice);
        builder.addEdge(lattice.firstOutEdge(vd[i], maskRaw));
        lattice.addEdge(vd[i], vd[i+1], *ai[i], tagToken, builder.build());
        delete ai[i];
    }
    delete [] ai;

}
