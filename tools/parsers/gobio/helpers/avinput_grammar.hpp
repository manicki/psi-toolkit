#ifndef AVINPUT_GRAMMAR_HPP_HDR
#define AVINPUT_GRAMMAR_HPP_HDR


#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_parse_tree.hpp>
#include <boost/spirit/include/classic_ast.hpp>
#include <boost/spirit/include/classic_tree_to_xml.hpp>


namespace classic = boost::spirit::classic;


struct avinput_grammar : public classic::grammar<avinput_grammar> {

    static const int attributeID = 1;
    static const int integerID = 2;
    static const int assignmentID = 3;
    static const int nodeID = 4;
    static const int symbolID = 5;
    static const int segmentID = 6;
    static const int specID = 7;
    static const int categoryID = 8;
    static const int extraID = 9;
    static const int scoreID = 10;

    template <typename ScannerT>
    struct definition {

        definition(avinput_grammar const & /*self*/) {

            // Start grammar definition

            integer
                = classic::lexeme_d[classic::leaf_node_d[classic::uint_p]];

            attribute
                = classic::lexeme_d[classic::leaf_node_d[
                    classic::upper_p >> *(classic::alpha_p | classic::digit_p)
                ]];

            score
                = classic::no_node_d[classic::ch_p('[')]
                >> classic::lexeme_d[classic::leaf_node_d[classic::real_p]]
                >> classic::no_node_d[classic::ch_p(']')];

            symbol
                = classic::lexeme_d[classic::leaf_node_d[
                    (
                        classic::lower_p
                        >> *(classic::lower_p | classic::digit_p | classic::ch_p('_'))
                    ) | (
                        classic::ch_p('\'')
                        >> *~classic::ch_p('\'')
                        >> classic::ch_p('\'')
                    )
                ]];

            assignment
                = attribute
                >> classic::no_node_d[classic::ch_p('=')]
                >> (integer | symbol)
                >> classic::no_node_d[classic::ch_p('\n')];


            category
                = classic::no_node_d[classic::ch_p('%')]
                >> classic::lexeme_d[classic::leaf_node_d[*~classic::ch_p('%')]]
                >> classic::no_node_d[classic::ch_p('%')];

            node
                = symbol
                >> !category
                >> !score
                >> classic::no_node_d[classic::ch_p('\n')]
                >> *assignment
                >> *extra;

            extra
                = classic::no_node_d[classic::ch_p('>')]
                >> classic::no_node_d[classic::ch_p('>')]
                >> !score
                >> classic::no_node_d[classic::ch_p('\n')]
                >> *assignment;

            segment
                = classic::no_node_d[classic::ch_p('!')]
                >> integer
                >> classic::no_node_d[classic::ch_p('-')]
                >> integer
                >> classic::no_node_d[classic::ch_p('\n')]
                >> *(classic::no_node_d[*classic::ch_p('\n')] >> node)
                >> classic::no_node_d[*classic::ch_p('\n')];

            spec
                = classic::no_node_d[*classic::ch_p('\n')]
                >> *segment
                >> classic::no_node_d[classic::end_p];

            // End grammar definition

            // turn on the debugging info.
            BOOST_SPIRIT_DEBUG_RULE(attribute);
            BOOST_SPIRIT_DEBUG_RULE(integer);
            BOOST_SPIRIT_DEBUG_RULE(assignment);
        }

        classic::rule< ScannerT, classic::parser_context<>, classic::parser_tag<attributeID> >
            attribute;
        classic::rule< ScannerT, classic::parser_context<>, classic::parser_tag<integerID> >
            integer;
        classic::rule< ScannerT, classic::parser_context<>, classic::parser_tag<assignmentID> >
            assignment;
        classic::rule< ScannerT, classic::parser_context<>, classic::parser_tag<nodeID> >
            node;
        classic::rule< ScannerT, classic::parser_context<>, classic::parser_tag<symbolID> >
            symbol;
        classic::rule< ScannerT, classic::parser_context<>, classic::parser_tag<segmentID> >
            segment;
        classic::rule< ScannerT, classic::parser_context<>, classic::parser_tag<specID> >
            spec;
        classic::rule< ScannerT, classic::parser_context<>, classic::parser_tag<categoryID> >
            category;
        classic::rule< ScannerT, classic::parser_context<>, classic::parser_tag<extraID> >
            extra;
        classic::rule< ScannerT, classic::parser_context<>, classic::parser_tag<scoreID> >
            score;

        classic::rule< ScannerT, classic::parser_context<>, classic::parser_tag<specID> > const &
            start() const { return spec; }

    };

};


#endif
