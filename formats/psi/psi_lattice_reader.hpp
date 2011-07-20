#ifndef PSI_LATTICE_READER_HDR
#define PSI_LATTICE_READER_HDR


#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi.hpp>

#include "lattice_reader.hpp"


using namespace boost::spirit;


struct PsiLRItem {
    int ordinal;
    int beginning;
    int length;
    std::string text;
    std::string tags;
    std::string annotationItem;
    std::string unused;

    void unescape() {
        text = LatticeRWUtils::unescape("Psi", text);
        tags = LatticeRWUtils::unescape("Psi", tags);
        annotationItem = LatticeRWUtils::unescape("Psi", annotationItem);
    }
};


BOOST_FUSION_ADAPT_STRUCT(
    PsiLRItem,
    (int, ordinal)
    // (std::string, unused) // strange error in spirit if there are to many string fields
    (int, beginning)
    (std::string, unused)
    (int, length)
    (std::string, unused)
    (std::string, text)
    (std::string, unused)
    (std::string, tags)
    (std::string, unused)
    (std::string, annotationItem)
)


struct PsiLRGrammar : public qi::grammar<std::string::const_iterator, PsiLRItem()> {

    PsiLRGrammar() : PsiLRGrammar::base_type(start) {

        start
            %= qi::int_
            >> ' ' //TODO
            >> qi::int_
            >> +(qi::space)
            >> qi::int_
            >> +(qi::space)
            >> +(qi::char_ - ' ')
            >> +(qi::space)
            >> +(qi::char_ - ' ')
            >> +(qi::space)
            >> +(qi::char_ - ' ')
            ;

    }

    qi::rule<std::string::const_iterator, PsiLRItem()> start;

};


class PsiLatticeReader : public LatticeReader {

public:
    /**
     * Reads from `inputStream` into `lattice`. If lattice is not empty,
     * new edges will be appended at the end of lattice.
     */
    void readIntoLattice(std::istream& inputStream, Lattice& lattice);

    /**
     * Gets format name (here: "Psi").
     */
    std::string getFormatName();

};


#endif
