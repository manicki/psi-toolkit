#ifndef AVINPUT_GRAMMAR_HPP_HDR
#define AVINPUT_GRAMMAR_HPP_HDR


#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_parse_tree.hpp>
#include <boost/spirit/include/classic_ast.hpp>
#include <boost/spirit/include/classic_tree_to_xml.hpp>


using namespace boost::spirit::classic;

struct avinput_grammar : public grammar<avinput_grammar>
{
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
    struct definition
    {
        definition(avinput_grammar const& /*self*/)
        {
            //  Start grammar definition
            integer     =   lexeme_d [ leaf_node_d [ uint_p ] ];

        attribute   =   lexeme_d[ leaf_node_d[
                         upper_p >> *(alpha_p | digit_p)
                         ] ];

        score = no_node_d[ch_p('[')]
        >> lexeme_d[leaf_node_d[real_p]]
        >> no_node_d[ch_p(']')];

        symbol = lexeme_d[ leaf_node_d [
                      (lower_p >> *(lower_p | digit_p | ch_p('_')))
                      |  (ch_p('\'') >> *~ch_p('\'') >> ch_p('\''))
                      ] ];

        assignment   =
        attribute >> no_node_d[ch_p('=')] >> (integer | symbol) >> no_node_d[ch_p('\n')]
        ;


        category =
        no_node_d[ch_p('%')] >> lexeme_d[leaf_node_d[*~ch_p('%')]] >> no_node_d[ch_p('%')];

        node =
        symbol >> !category >> !score >> no_node_d[ch_p('\n')] >> *assignment >> *extra;

        extra =
        no_node_d[ch_p('>')] >> no_node_d[ch_p('>')] >> !score >> no_node_d[ch_p('\n')] >>
        *assignment;

        segment =
        no_node_d[ch_p('!')] >> integer >> no_node_d[ch_p('-')]
                     >> integer >> no_node_d[ch_p('\n')]
              >> *(no_node_d[*ch_p('\n')] >> node) >> no_node_d[*ch_p('\n')];

        spec = no_node_d[*ch_p('\n')] >> *segment >> no_node_d[end_p];

            //  End grammar definition

            // turn on the debugging info.
            BOOST_SPIRIT_DEBUG_RULE(attribute);
            BOOST_SPIRIT_DEBUG_RULE(integer);
            BOOST_SPIRIT_DEBUG_RULE(assignment);
        }

        rule<ScannerT, parser_context<>, parser_tag<attributeID> >   attribute;
        rule<ScannerT, parser_context<>, parser_tag<integerID> >         integer;
        rule<ScannerT, parser_context<>, parser_tag<assignmentID> > assignment;
        rule<ScannerT, parser_context<>, parser_tag<nodeID> > node;
        rule<ScannerT, parser_context<>, parser_tag<symbolID> > symbol;
        rule<ScannerT, parser_context<>, parser_tag<segmentID> > segment;
        rule<ScannerT, parser_context<>, parser_tag<specID> > spec;
        rule<ScannerT, parser_context<>, parser_tag<categoryID> > category;
        rule<ScannerT, parser_context<>, parser_tag<extraID> > extra;
        rule<ScannerT, parser_context<>, parser_tag<scoreID> > score;

        rule<ScannerT, parser_context<>, parser_tag<specID> > const&
        start() const { return spec; }
    };
};


#endif

