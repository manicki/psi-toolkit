#ifndef PSI_LATTICE_READER_HDR
#define PSI_LATTICE_READER_HDR

// for unknown reasons property_tree has to be included
// before Spirit/Lambda (at least for GCC 4.6.1 &
// Boost 1.47.0 at Arch Linux)
#include <boost/property_tree/ptree.hpp>

#include <boost/foreach.hpp>
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

#include "logging.hpp"


namespace qi = boost::spirit::qi;


struct PsiLRAnnotation {
    std::string category;
    double score;
    std::string avVector;
    std::string partition;

    void unescape(Quoter & quoter) {
        category = quoter.unescape(category);
        avVector = quoter.unescape(avVector);
        partition = quoter.unescape(partition);
    }
};


BOOST_FUSION_ADAPT_STRUCT(
    PsiLRAnnotation,
    (std::string, category)
    (double, score)
    (std::string, avVector)
    (std::string, partition)
)


struct PsiLRItem {
    int ordinal;
    bool beginningLoose;
    int beginning;
    bool lengthPoint;
    bool lengthLoose;
    int length;
    std::string text;
    std::vector<std::string> tags;
    std::string annotationText;
    PsiLRAnnotation annotationItem;

    void unescape(Quoter & quoter) {
        text = quoter.unescape(text);
        BOOST_FOREACH(std::string tag, tags) {
            tag = quoter.unescape(tag);
        }
        annotationItem.unescape(quoter);
    }
};


BOOST_FUSION_ADAPT_STRUCT(
    PsiLRItem,
    (int, ordinal)
    (bool, beginningLoose)
    (int, beginning)
    (bool, lengthPoint)
    (bool, lengthLoose)
    (int, length)
    (std::string, text)
    (std::vector<std::string>, tags)
    (std::string, annotationText)
    (PsiLRAnnotation, annotationItem)
)


struct PsiLRGrammar : public qi::grammar<std::string::const_iterator, PsiLRItem()> {

    PsiLRGrammar() : PsiLRGrammar::base_type(start) {

        start
            %= qi::int_
            >> whitespaces
            >> loose
            >> qi::int_
            >> whitespaces
            >> point
            >> loose
            >> qi::int_
            >> whitespaces
            >> +(qi::char_ - ' ')
            >> whitespaces
            >> tags
            >> whitespaces
            >> -(+(qi::char_ - ' ') >> whitespaces)
            >> annotation
            ;

        whitespaces = +(qi::space);

        loose
            = qi::eps[qi::_val = false]
            >> -(qi::lit("@")[qi::_val = true])
            ;

        point
            = qi::eps[qi::_val = false]
            >> -(qi::lit("*")[qi::_val = true])
            ;

        tags
            %= +(qi::char_ - ' ' - ',') % ','
            ;

        annotation
            %= +(qi::char_ - ' ' - ',' - '[' - '<')
            >> score
            >> -(',' >> +(qi::char_ - '['))
            >> partition
            ;

        score
            = qi::eps[qi::_val = 0.0]
            >> -('<' >> qi::double_[qi::_val = qi::_1] >> '>')
            ;

        partition
            %= -(qi::char_("[") >> *(qi::char_ - ']') >> qi::char_("]"))
            ;

    }

    qi::rule<std::string::const_iterator, PsiLRItem()> start;
    qi::rule<std::string::const_iterator, qi::unused_type()> whitespaces;
    qi::rule<std::string::const_iterator, bool()> loose;
    qi::rule<std::string::const_iterator, bool()> point;
    qi::rule<std::string::const_iterator, std::vector<std::string>()> tags;
    qi::rule<std::string::const_iterator, PsiLRAnnotation()> annotation;
    qi::rule<std::string::const_iterator, double()> score;
    qi::rule<std::string::const_iterator, std::string()> partition;

};


struct PsiLRAVPair {
    std::string arg;
    std::string val;
};


struct PsiLRAVGrammar : public qi::grammar<
    std::string::const_iterator,
    std::vector<std::string>()
> {

    PsiLRAVGrammar() : PsiLRAVGrammar::base_type(start) {

        start
            %= +(qi::char_ - ',') % ','
            ;

    }

    qi::rule<std::string::const_iterator, std::vector<std::string>()> start;

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
