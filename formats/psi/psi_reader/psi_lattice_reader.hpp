#ifndef PSI_LATTICE_READER_HDR
#define PSI_LATTICE_READER_HDR

#include <iomanip>

// for unknown reasons property_tree has to be included
// before Spirit/Lambda (at least for GCC 4.6.1 &
// Boost 1.47.0 at Arch Linux)
#include <boost/property_tree/ptree.hpp>

#include <boost/algorithm/string/trim.hpp>
#include <boost/foreach.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi.hpp>

#include "stream_lattice_reader.hpp"
#include "lattice_reader_factory.hpp"
#include "quoter.hpp"
#include "psi_quoter.hpp"

#include "logging.hpp"


namespace qi = boost::spirit::qi;


struct StringSequenceGrammar : public qi::grammar<
    std::string::const_iterator,
    std::vector<std::string>()
> {

    StringSequenceGrammar() : StringSequenceGrammar::base_type(start) {

        start
            %= +(qi::char_ - ' ') % whitespaces
            ;

        whitespaces = +(qi::space);

    }

    qi::rule<std::string::const_iterator, std::vector<std::string>()> start;
    qi::rule<std::string::const_iterator, qi::unused_type()> whitespaces;

};

struct PsiLRAnnotation {
    std::string category;
    double score;
    std::string avVector;
    std::string partitions;

    void unescape(Quoter & quoter) {
        category = quoter.unescape(category);
        avVector = quoter.unescape(avVector);
        partitions = quoter.unescape(partitions);
    }
};

BOOST_FUSION_ADAPT_STRUCT(
    PsiLRAnnotation,
    (std::string, category)
    (double, score)
    (std::string, avVector)
    (std::string, partitions)
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
        annotationText = quoter.unescape(annotationText);
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


struct PsiLRGrammar : public qi::grammar<
    std::string::const_iterator,
    PsiLRItem()
> {

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
            %= +(qi::char_ - ' ' - ',' - '[' - '<')                         //category
            >> score                                                        //score
            >> -(',' >> +(qi::char_ - '['))                                 //av
            >> -(qi::char_("[") >> *(qi::char_ - ']') >> qi::char_("]"))    //partitions
            ;

        score
            = qi::eps[qi::_val = 0.0]
            >> -('<' >> qi::double_[qi::_val = qi::_1] >> '>')
            ;

    }

    qi::rule<std::string::const_iterator, PsiLRItem()> start;
    qi::rule<std::string::const_iterator, qi::unused_type()> whitespaces;
    qi::rule<std::string::const_iterator, bool()> loose;
    qi::rule<std::string::const_iterator, bool()> point;
    qi::rule<std::string::const_iterator, std::vector<std::string>()> tags;
    qi::rule<std::string::const_iterator, PsiLRAnnotation()> annotation;
    qi::rule<std::string::const_iterator, double()> score;

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


struct PsiLRAVPairItem {
    std::string arg;
    std::string val;
};


BOOST_FUSION_ADAPT_STRUCT(
    PsiLRAVPairItem,
    (std::string, arg)
    (std::string, val)
)


struct PsiLRAVPairGrammar : public qi::grammar<
    std::string::const_iterator,
    PsiLRAVPairItem()
> {

    PsiLRAVPairGrammar() : PsiLRAVPairGrammar::base_type(start) {

        start
            %= +(qi::char_ - ',' - '=')
            >> '='
            >> +(qi::char_ - ',' - '=')
            ;

    }

    qi::rule<std::string::const_iterator, PsiLRAVPairItem()> start;

};


struct PsiLRPartitionsGrammar : public qi::grammar<
    std::string::const_iterator,
    std::vector<std::string>()
> {

    PsiLRPartitionsGrammar() : PsiLRPartitionsGrammar::base_type(start) {

        start
            %= '['
            >> partition % ','
            >> ']'
            ;

        partition
            = qi::eps[qi::_val = ""]
            >> +(qi::char_ - '(' - '<' - ']' - ',')[qi::_val += qi::_1]
            >> -(
                qi::char_('(')[qi::_val += qi::_1]
                >> +(qi::char_ - ')')[qi::_val += qi::_1]
                >> qi::char_(')')[qi::_val += qi::_1]
                )
            >> -(
                qi::char_('<')[qi::_val += qi::_1]
                >> +(qi::char_ - '>')[qi::_val += qi::_1]
                >> qi::char_('>')[qi::_val += qi::_1]
                )
            ;

    }

    qi::rule<std::string::const_iterator, std::vector<std::string>()> start;
    qi::rule<std::string::const_iterator, std::string()> partition;

};


struct PsiLRPartitionItem {
    std::vector<int> edgeNumbers;
    std::vector<std::string> tags;
    boost::optional<double> score;
};


BOOST_FUSION_ADAPT_STRUCT(
    PsiLRPartitionItem,
    (std::vector<int>, edgeNumbers)
    (std::vector<std::string>, tags)
    (boost::optional<double>, score)
)


struct PsiLRPartitionGrammar : public qi::grammar<
    std::string::const_iterator,
    PsiLRPartitionItem()
> {

    PsiLRPartitionGrammar() : PsiLRPartitionGrammar::base_type(start) {

        start
            %= edge % '-'
            >> tags
            >> score
            ;

        edge
            = qi::eps[qi::_val = 0]
            >> -(qi::int_[qi::_val = qi::_1])
            ;

        tags
            %= -('(' >> +(qi::char_ - ')' - ' ' - ',') % ',' >> ')')
            ;

        score
            %= -('<' >> qi::double_ >> '>')
            ;

    }

    qi::rule<std::string::const_iterator, PsiLRPartitionItem()> start;
    qi::rule<std::string::const_iterator, int()> edge;
    qi::rule<std::string::const_iterator, std::vector<std::string>()> tags;
    qi::rule<std::string::const_iterator, boost::optional<double>()> score;

};


struct PsiLRPartitionElements {
    Lattice::EdgeSequence::Builder builder;
    LayerTagCollection tags;
    Lattice::Score score;

    PsiLRPartitionElements(
        Lattice::EdgeSequence::Builder aBuilder,
        LayerTagCollection aTags,
        Lattice::Score aScore
    ) :
        builder(aBuilder),
        tags(aTags),
        score(aScore)
    { }
};


class PsiLatticeReader : public StreamLatticeReader {

public:

    /**
     * Gets format name (here: "Psi").
     */
    std::string getFormatName();

    class Factory : public LatticeReaderFactory<std::istream> {
    public:
        virtual ~Factory();

    private:
        virtual LatticeReader<std::istream>* doCreateLatticeReader(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName();
        virtual boost::filesystem::path doGetFile();
    };

private:
    virtual std::string doInfo();

    class Worker : public ReaderWorker<std::istream> {
    public:
        Worker(PsiLatticeReader& processor,
               std::istream& inputStream,
               Lattice& lattice);

        virtual void doRun();

    private:
        PsiLatticeReader& processor_;
    };

    virtual ReaderWorker<std::istream>* doCreateReaderWorker(
        std::istream& inputStream, Lattice& lattice) {

        return new Worker(*this, inputStream, lattice);
    }

};


#endif
