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

    namespace fusion = boost::fusion;
    namespace phoenix = boost::phoenix;
    namespace qi = boost::spirit::qi;
    namespace unicode = boost::spirit::ascii;

    template <typename Iterator, typename FSA>
    struct NDFSAGrammar : qi::grammar<Iterator, FSA(), unicode::space_type> {
        
        NDFSAGrammar() : NDFSAGrammar::base_type(exp) {
            using qi::lit;
            using qi::lexeme;
            using qi::eps;
            using qi::alnum;
            using qi::int_;
            using unicode::char_;
            using namespace qi::labels;
            //
            exp =
                term [qi::_val = qi::_1] >>
                *(
                  term [phoenix::bind(&concatenate<FSA, FSA>, qi::_val, qi::_1)]
                  | ( '|' >> term [phoenix::bind(&unify<FSA, FSA>, qi::_val, qi::_1)] )
                );
            
            term = 
                ( factor >> "*" ) [phoenix::bind(&kleene_star<FSA>, qi::_val = qi::_1)]
                | ( factor >> "+" ) [phoenix::bind(&kleene_plus<FSA>, qi::_val = qi::_1)]
                | ( factor >> "?" ) [phoenix::bind(&kleene_option<FSA>, qi::_val = qi::_1)]
                | factor [qi::_val = qi::_1];
                
            factor = (
            //    any [ phoenix::bind( &psi::QueryFSA::from_symbol, qi::_val, psi::ANY ) ]
            //|
                symbol [ qi::_val = phoenix::construct<FSA>( phoenix::begin(qi::_1), phoenix::end(qi::_1) ) ]
            /*|
                regexp [
                    phoenix::bind(
                        &psi::QueryFSA::from_symbols, qi::_val,
                        phoenix::bind(
                            &psi::query::RuleSymbolNumberMap::get_numbers_by_regex,
                            sm, qi::_1
                        )
                    )
                ]*/
            |
                '(' >> exp [qi::_val = qi::_1] >> ')'
            );
    
            symbol %= lexeme[+alnum];
            //any %= lexeme[+char_('_')];
            //regexp %= lit('/') >> lexeme[+(char_ - '/')] >> lit('/');
        }
    
        qi::rule<Iterator, FSA(), unicode::space_type> exp;
        qi::rule<Iterator, FSA(), unicode::space_type> term;
        qi::rule<Iterator, FSA(), unicode::space_type> factor;
        qi::rule<Iterator, std::string(), unicode::space_type> symbol;
        qi::rule<Iterator, std::string(), unicode::space_type> any;
        qi::rule<Iterator, std::string(), unicode::space_type> regexp;
        
    };


}

#endif
