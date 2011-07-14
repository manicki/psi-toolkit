#ifndef UTT_READER_HDR
#define UTT_READER_HDR

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi.hpp>

#include "lattice_reader.hpp"


using namespace boost::spirit;


struct UTTReaderItem {
    int position;
    int length;
    std::string segmentType;
    std::string form;
    std::string annotations;
};


BOOST_FUSION_ADAPT_STRUCT(
    UTTReaderItem,
    (int, position)
    (int, length)
    (std::string, segmentType)
    (std::string, form)
    (std::string, annotations)
)


struct UTTReaderGrammar : public qi::grammar<std::string::const_iterator, UTTReaderItem()> {
    
    UTTReaderGrammar() : UTTReaderGrammar::base_type(start) {
        
        start 
            %= qi::int_ 
            >> ' '
            >> qi::int_ 
            >> ' '
            >> +(qi::char_ - ' ')
            >> ' ' 
            >> +(qi::char_ - ' ')
            >> qi::lexeme[' ' >> +(qi::char_)]
            ;
            
    }
    
    qi::rule<std::string::const_iterator, UTTReaderItem()> start;
    
};


class UTTReader : public LatticeReader {

public:
    /**
     * Reads from `inputStream` into `lattice`. If lattice is not empty,
     * new edges will be appended at the end of lattice.
     */
    void readIntoLattice(std::istream& inputStream, Lattice& lattice);

    /**
     * Gets format name (here: "UTT").
     */
    std::string getFormatName();
};


#endif
