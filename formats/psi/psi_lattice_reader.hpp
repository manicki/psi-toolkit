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
#include "quoter.hpp"
#include "psi_quoter.hpp"


using namespace boost::spirit;


struct PsiLRItem {
    int ordinal;
    int beginning;
    int length;
    std::string text;
    std::string tags;
    std::string annotationItem;
    std::string unused;

    void unescape(Quoter & quoter) {
        text = quoter.unescape(text);
        tags = quoter.unescape(tags);
        annotationItem = quoter.unescape(annotationItem);
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
     * Gets format name (here: "Psi").
     */
    std::string getFormatName();

private:
    virtual std::string doInfo();

    class Worker : public ReaderWorker {
    public:
        Worker(PsiLatticeReader& processor,
               std::istream& inputStream,
               Lattice& lattice);

        virtual void doRun();

    private:
        PsiLatticeReader& processor_;
    };

    virtual ReaderWorker* doCreateReaderWorker(std::istream& inputStream, Lattice& lattice) {
        return new Worker(*this, inputStream, lattice);
    }

};


#endif
