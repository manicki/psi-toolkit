#include "tests.hpp"

#include <fstream>
#include <set>
#include <string>

#include <boost/algorithm/string/trim.hpp>
#include <boost/scoped_ptr.hpp>

#include "lattice_preparators.hpp"
#include "gv_lattice_writer.hpp"


BOOST_AUTO_TEST_SUITE( gv_lattice_writer )


BOOST_AUTO_TEST_CASE( gv_lattice_writer_simple ) {

    AnnotationItemManager aim;
    Lattice lattice(aim);
    lattice_preparators::prepareSimpleLattice(lattice);

    std::set<std::string> filter;

    boost::scoped_ptr<LatticeWriter<std::ostream> > writer(new GVLatticeWriter(
        true, // show tags
        false, // color
        filter, // filter
        "dot", // output format
        false, // tree
        false // align
    ));

    BOOST_CHECK_EQUAL(writer->getFormatName(), "GraphViz");
    BOOST_CHECK_EQUAL(writer->info(), "GraphViz writer");

    std::stringstream outSs;
    writer->writeLattice(lattice, outSs);
    std::string refLine;
    std::string outLine;
    std::ifstream reference(ROOT_DIR "formats/gv/t/files/simple.dot");
    int linesCount = 0;
    while (getline(reference, refLine)) {
        if (refLine.find("pos") == std::string::npos) continue;
        BOOST_CHECK(getline(outSs, outLine));
        while (outLine.find("pos") == std::string::npos) {
            BOOST_CHECK(getline(outSs, outLine));
        }
        BOOST_CHECK_EQUAL(
            boost::algorithm::trim_copy(refLine.substr(0, refLine.find("pos"))),
            boost::algorithm::trim_copy(outLine.substr(0, outLine.find("pos")))
        );
        ++linesCount;
    }
    BOOST_CHECK_EQUAL(linesCount, 32);

}


BOOST_AUTO_TEST_CASE( gv_lattice_writer_advanced ) {

    AnnotationItemManager aim;
    Lattice lattice(aim);
    lattice_preparators::prepareAdvancedLattice(lattice);

    std::set<std::string> filter;

    boost::scoped_ptr<LatticeWriter<std::ostream> > writer(new GVLatticeWriter(
        true, // show tags
        false, // color
        filter, // filter
        "dot", // output format
        false, // tree
        false // align
    ));

    std::stringstream outSs;
    writer->writeLattice(lattice, outSs);
    std::string refLine;
    std::string outLine;
    std::ifstream reference(ROOT_DIR "formats/gv/t/files/advanced.dot");
    int linesCount = 0;
    while (getline(reference, refLine)) {
        if (refLine.find("pos") == std::string::npos) continue;
        BOOST_CHECK(getline(outSs, outLine));
        while (outLine.find("pos") == std::string::npos) {
            BOOST_CHECK(getline(outSs, outLine));
        }
        BOOST_CHECK_EQUAL(
            boost::algorithm::trim_copy(refLine.substr(0, refLine.find("pos"))),
            boost::algorithm::trim_copy(outLine.substr(0, outLine.find("pos")))
        );
        ++linesCount;
    }
    BOOST_CHECK_EQUAL(linesCount, 45);

}


BOOST_AUTO_TEST_CASE( dot_lattice_writer_tree ) {

    AnnotationItemManager aim;
    Lattice lattice(aim);
    lattice_preparators::prepareRegularLattice(lattice);

    std::set<std::string> filter;

    boost::scoped_ptr<LatticeWriter<std::ostream> > writer(new GVLatticeWriter(
        false, // show tags
        false, // color
        filter, // filter
        "dot", // output format
        true, // tree
        false // align
    ));

    std::stringstream outSs;
    writer->writeLattice(lattice, outSs);
    std::string refLine;
    std::string outLine;
    std::ifstream reference(ROOT_DIR "formats/gv/t/files/tree.dot");
    int linesCount = 0;
    while (getline(reference, refLine)) {
        if (refLine.find("pos") == std::string::npos) continue;
        BOOST_CHECK(getline(outSs, outLine));
        while (outLine.find("pos") == std::string::npos) {
            BOOST_CHECK(getline(outSs, outLine));
        }
        BOOST_CHECK_EQUAL(
            boost::algorithm::trim_copy(refLine.substr(0, refLine.find("pos"))),
            boost::algorithm::trim_copy(outLine.substr(0, outLine.find("pos")))
        );
        ++linesCount;
    }
    BOOST_CHECK_EQUAL(linesCount, 237);

}


BOOST_AUTO_TEST_CASE( dot_lattice_writer_tree_canon ) {

    AnnotationItemManager aim;
    Lattice lattice(aim);
    lattice_preparators::prepareRegularLattice(lattice);

    std::set<std::string> filter;

    boost::scoped_ptr<LatticeWriter<std::ostream> > writer(new GVLatticeWriter(
        false, // show tags
        false, // color
        filter, // filter
        "canon", // output format
        true, // tree
        false // align
    ));

    std::ostringstream osstr;
    writer->writeLattice(lattice, osstr);

    std::string line;
    std::string contents;
    std::ifstream s(ROOT_DIR "formats/gv/t/files/tree_canon.dot");
    while (getline(s, line)) {
        contents += line;
        contents += "\n";
    }

    BOOST_CHECK_EQUAL(osstr.str(), contents);

}


BOOST_AUTO_TEST_SUITE_END()
