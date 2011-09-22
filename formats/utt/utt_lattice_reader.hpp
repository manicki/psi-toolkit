#ifndef UTT_LATTICE_READER_HDR
#define UTT_LATTICE_READER_HDR

// for unknown reasons property_tree has to be included
// before Spirit/Lambda (at least for GCC 4.6.1 &
// Boost 1.47.0 at Arch Linux)
#include <boost/property_tree/ptree.hpp>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi.hpp>

#include "lattice_reader.hpp"
#include "lattice_reader_factory.hpp"
#include "quoter.hpp"
#include "utt_quoter.hpp"


namespace qi = boost::spirit::qi;


struct UTTLRItem {
    int position;
    int length;
    std::string segmentType;
    std::string form;
    std::string annotations;

    void unescape(Quoter & quoter) {
        segmentType = quoter.unescape(segmentType);
        form = quoter.unescape(form);
        annotations = quoter.unescape(annotations);
    }
};


BOOST_FUSION_ADAPT_STRUCT(
    UTTLRItem,
    (int, position)
    (int, length)
    (std::string, segmentType)
    (std::string, form)
    (std::string, annotations)
)


struct UTTLRGrammar : public qi::grammar<std::string::const_iterator, UTTLRItem()> {

    UTTLRGrammar() : UTTLRGrammar::base_type(start) {

        start
            %= -whitespaces
            >> optionalInt
            >> optionalInt
            >> +(qi::char_ - ' ')
            >> whitespaces
            >> +(qi::char_ - ' ')
            >> -(whitespaces >> +(qi::char_))
            ;

        optionalInt
            = qi::eps[qi::_val = -1]
            >> -(qi::int_[qi::_val = qi::_1] >> +(qi::space))
            ;

        whitespaces = +(qi::space);

    }

    qi::rule<std::string::const_iterator, UTTLRItem()> start;
    qi::rule<std::string::const_iterator, int()> optionalInt;
    qi::rule<std::string::const_iterator, qi::unused_type()> whitespaces;

};


class UTTLatticeReader : public LatticeReader {

public:

    /**
     * Gets format name (here: "UTT").
     */
    std::string getFormatName();

    virtual ~UTTLatticeReader();

    class Factory : public LatticeReaderFactory {
    public:
        virtual ~Factory();

    private:
        virtual LatticeReader* doCreateLatticeReader(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName();
    };

private:
    virtual std::string doInfo();

    class Worker : public ReaderWorker {
    public:
        Worker(UTTLatticeReader& processor,
               std::istream& inputStream,
               Lattice& lattice);

        virtual void doRun();

    private:
        UTTLatticeReader& processor_;
    };

    virtual ReaderWorker* doCreateReaderWorker(std::istream& inputStream, Lattice& lattice) {
        return new Worker(*this, inputStream, lattice);
    }
};


#endif
