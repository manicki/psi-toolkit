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

#include "utf8/utf8.h"

#include "stream_lattice_reader.hpp"
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


struct UTTLRGrammar : public qi::grammar<
    std::string::const_iterator,
    UTTLRItem()
> {

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


struct UTTLRAnnotationsGrammar : public qi::grammar<
    std::string::const_iterator,
    std::vector<std::string>()
> {

    UTTLRAnnotationsGrammar() : UTTLRAnnotationsGrammar::base_type(start) {

        start
            %= +(qi::char_- ' ') % whitespaces
            ;

        whitespaces = +(qi::space);

    }

    qi::rule<std::string::const_iterator, std::vector<std::string>()> start;
    qi::rule<std::string::const_iterator, qi::unused_type()> whitespaces;

};


struct UTTLRAVItem {
    std::string arg;
    std::string val;
};


BOOST_FUSION_ADAPT_STRUCT(
    UTTLRAVItem,
    (std::string, arg)
    (std::string, val)
)


struct UTTLRAVGrammar : public qi::grammar<
    std::string::const_iterator,
    UTTLRAVItem()
> {

    UTTLRAVGrammar() : UTTLRAVGrammar::base_type(start) {

        start
            %= +(qi::char_ - ' ' - ':')
            >> ':'
            >> +(qi::char_ - ' ')
            ;

    }

    qi::rule<std::string::const_iterator, UTTLRAVItem()> start;

};


class UTTLatticeReader : public StreamLatticeReader {

public:

    /**
     * Gets format name (here: "UTT").
     */
    std::string getFormatName();

    virtual ~UTTLatticeReader();

    class Factory : public LatticeReaderFactory<std::istream> {
    public:
        virtual ~Factory();

    private:
        virtual LatticeReader<std::istream>* doCreateLatticeReader(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName();
    };

private:
    virtual std::string doInfo();

    class PosConverter {
    public:
        PosConverter() : count_(0) { }

        int psi(int uttPos) {
            for (int i = position_.size() - 1; i >= 0; --i) {
                if (position_[i] <= uttPos) {
                    return uttPos + offset_[i];
                }
            }
            return uttPos;
        }

        int utt(int psiPos) {
            for (int i = position_.size() - 1; i >= 0; --i) {
                if (position_[i] + offset_[i] <= psiPos) {
                    return psiPos - offset_[i];
                }
            }
            return psiPos;
        }

        void add(int uttPos, int psiPos) {
            if (psiPos - uttPos > count_) {
                count_ = psiPos - uttPos;
                position_.push_back(uttPos);
                offset_.push_back(count_);
            }
        }

    private:
        std::vector<int> position_;
        std::vector<int> offset_;
        int count_;
    };

    class Worker : public ReaderWorker<std::istream> {
    public:
        Worker(UTTLatticeReader& processor,
               std::istream& inputStream,
               Lattice& lattice);

        virtual void doRun();

    private:
        UTTLatticeReader& processor_;
    };

    virtual ReaderWorker<std::istream>* doCreateReaderWorker(std::istream& inputStream, Lattice& lattice) {
        return new Worker(*this, inputStream, lattice);
    }
};


#endif
