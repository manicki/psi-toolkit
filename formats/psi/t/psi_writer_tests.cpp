#include "tests.hpp"

#include <fstream>

#include <boost/scoped_ptr.hpp>

#include "lattice_preparators.hpp"
#include "psi_lattice_writer.hpp"


BOOST_AUTO_TEST_SUITE( psi_lattice_writer )


BOOST_AUTO_TEST_CASE( psi_lattice_writer_simple ) {
    Lattice lattice;
    lattice_preparators::prepareSimpleLattice(lattice);

    boost::scoped_ptr<LatticeWriter<std::ostream> > writer(new PsiLatticeWriter());

    std::ostringstream osstr;
    writer->writeLattice(lattice, osstr);

    std::string line;
    std::string contents;
    std::ifstream s(ROOT_DIR "formats/psi/t/files/pl_sample_simple.txt");
    while (getline(s, line)) {
        contents += line;
        contents += "\n";
    }

    BOOST_CHECK_EQUAL(osstr.str(), contents);

}


BOOST_AUTO_TEST_CASE( psi_lattice_writer_advanced ) {
    Lattice lattice;
    lattice_preparators::prepareAdvancedLattice(lattice);

    boost::scoped_ptr<LatticeWriter<std::ostream> > writer(new PsiLatticeWriter());

    std::ostringstream osstr;
    writer->writeLattice(lattice, osstr);

    std::string line;
    std::string contents;
    std::ifstream s(ROOT_DIR "formats/psi/t/files/pl_sample_nocomments.txt");
    while (getline(s, line)) {
        contents += line;
        contents += "\n";
    }

    BOOST_CHECK_EQUAL(osstr.str(), contents);
}


BOOST_AUTO_TEST_CASE( psi_lattice_writer_regular ) {
    Lattice lattice;
    lattice_preparators::prepareRegularLattice(lattice);

    boost::scoped_ptr<LatticeWriter<std::ostream> > writer(new PsiLatticeWriter());

    std::ostringstream osstr;
    writer->writeLattice(lattice, osstr);

    std::string line;
    std::string contents;
    std::ifstream s(ROOT_DIR "formats/psi/t/files/regular.txt");
    while (getline(s, line)) {
        contents += line;
        contents += "\n";
    }

    BOOST_CHECK_EQUAL(osstr.str(), contents);
}


BOOST_AUTO_TEST_CASE( psi_lattice_writer_one_symbol_tokens ) {
    Lattice lattice;
    lattice_preparators::prepareLatticeWithOneSymbolTokens(lattice, "abccdddd");

    boost::scoped_ptr<LatticeWriter<std::ostream> > writer(new PsiLatticeWriter());

    std::ostringstream osstr;
    writer->writeLattice(lattice, osstr);

    std::string line;
    std::string contents;
    std::ifstream s(ROOT_DIR "formats/psi/t/files/one_symbol_tokens.txt");
    while (getline(s, line)) {
        contents += line;
        contents += "\n";
    }

    BOOST_CHECK_EQUAL(osstr.str(), contents);
}


BOOST_AUTO_TEST_CASE( psi_lattice_writer_planes ) {
    Lattice lattice;
    lattice_preparators::prepareSimpleLattice(lattice);

    Lattice::VertexDescriptor pre_ala = lattice.getFirstVertex();
    Lattice::VertexDescriptor post_ala = lattice.getVertexForRawCharIndex(3);
    Lattice::VertexDescriptor pre_ma = lattice.getVertexForRawCharIndex(4);
    Lattice::VertexDescriptor post_ma = lattice.getVertexForRawCharIndex(6);
    Lattice::VertexDescriptor pre_slonia = lattice.getVertexForRawCharIndex(7);
    Lattice::VertexDescriptor post_slonia = lattice.getLastVertex();

    LayerTagCollection
        tagTranslation = lattice.getLayerTagManager().createSingletonTagCollection("!translation");
    LayerTagCollection
        tagToken = lattice.getLayerTagManager().createSingletonTagCollection("token");
    LayerTagMask maskToken = lattice.getLayerTagManager().getMask(tagToken);

    AnnotationItem aiWord("word");
    lattice.getAnnotationItemManager().setValue(aiWord, "type", "word");

    BOOST_CHECK(!lattice.getLayerTagManager().areInTheSamePlane(tagTranslation, tagToken));

    Lattice::EdgeSequence::Builder ala_builder(lattice);
    ala_builder.addEdge(lattice.firstOutEdge(pre_ala, maskToken));
    lattice.addEdge(pre_ala, post_ala, aiWord, tagTranslation, ala_builder.build());

    Lattice::EdgeSequence::Builder ma_builder(lattice);
    ma_builder.addEdge(lattice.firstOutEdge(pre_ma, maskToken));
    lattice.addEdge(pre_ma, post_ma, aiWord, tagTranslation, ma_builder.build());

    Lattice::EdgeSequence::Builder slonia_builder(lattice);
    slonia_builder.addEdge(lattice.firstOutEdge(pre_slonia, maskToken));
    lattice.addEdge(pre_slonia, post_slonia, aiWord, tagTranslation, slonia_builder.build());

    boost::scoped_ptr<LatticeWriter<std::ostream> > writer(new PsiLatticeWriter());

    std::ostringstream osstr;
    writer->writeLattice(lattice, osstr);

    std::string line;
    std::string contents;
    std::ifstream s(ROOT_DIR "formats/psi/t/files/pl_sample_planes.txt");
    while (getline(s, line)) {
        contents += line;
        contents += "\n";
    }

    BOOST_CHECK_EQUAL(osstr.str(), contents);
}


BOOST_AUTO_TEST_SUITE_END()
