#ifndef QUERYGRAMMARS_HPP__
#define QUERYGRAMMARS_HPP__

#include <string>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/spirit/home/phoenix/bind/bind_function.hpp>
#include <boost/spirit/home/phoenix/bind/bind_member_function.hpp>
#include <boost/spirit/home/phoenix/stl/container.hpp>

#include "NDFSA.hpp"
#include "Algorithms.hpp"

namespace psi {

    namespace phoenix = boost::phoenix;
    namespace qi = boost::spirit::qi;
    namespace unicode = boost::spirit::standard;

    template <typename Iterator, typename FSA>
    struct CharGrammar : qi::grammar<Iterator, FSA()> {

        CharGrammar() : CharGrammar::base_type(alternation) {

            alternation =
                sequence [qi::_val = qi::_1] >>
                *( '|' >> sequence [phoenix::bind(&unify<FSA, FSA>, qi::_val, qi::_1)] );

            sequence =
                repetition [qi::_val = qi::_1] >>
                *( repetition [phoenix::bind(&concatenate<FSA, FSA>, qi::_val, qi::_1)] );

            repetition =
                ( factor >> "*" ) [phoenix::bind(&kleene_star<FSA>, qi::_val = qi::_1)]
                | ( factor >> "+" ) [phoenix::bind(&kleene_plus<FSA>, qi::_val = qi::_1)]
                | ( factor >> "?" ) [phoenix::bind(&kleene_option<FSA>, qi::_val = qi::_1)]
                | factor [qi::_val = qi::_1];

            factor =
                symbol [ qi::_val =
                    phoenix::construct<FSA>( phoenix::begin(qi::_1), phoenix::end(qi::_1) ) ]
                | '(' >> alternation [qi::_val = qi::_1] >> ')';

            symbol = ( ~unicode::char_("()|+*?") ) [qi::_val = qi::_1];

        }

        qi::rule<Iterator, FSA()> alternation;
        qi::rule<Iterator, FSA()> sequence;
        qi::rule<Iterator, FSA()> repetition;
        qi::rule<Iterator, FSA()> factor;
        qi::rule<Iterator, std::string()> symbol;

    };


}

#endif
