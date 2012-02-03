#include "tests.hpp"

#include "simple_cfg_combinator.tpl"


BOOST_AUTO_TEST_SUITE( gobio_parser )


BOOST_AUTO_TEST_CASE( combinator ) {

    simple_cfg_combinator<char, simple_cfg_rule<char> > combinator;

    combinator.add_unary_rule('X', simple_cfg_rule<char>('S', 1.0));
    combinator.add_binary_rule('a', 'Y', simple_cfg_rule<char>('X', 1.0));
    combinator.add_binary_rule('c', 'Y', simple_cfg_rule<char>('X', 1.0));
    combinator.add_binary_rule('c', 'Y', simple_cfg_rule<char>('S', 1.0));
    combinator.add_binary_rule('X', 'b', simple_cfg_rule<char>('Y', 1.0));
    combinator.add_binary_rule('X', 'd', simple_cfg_rule<char>('Y', 1.0));
    combinator.add_unary_rule('b', simple_cfg_rule<char>('Y', 1.0));
    combinator.add_binary_rule('c', 'Y', simple_cfg_rule<char>('S', 2.0));
    combinator.add_unary_rule('b', simple_cfg_rule<char>('T', 1.0));

    BOOST_CHECK_EQUAL(combinator.combine_unary('b').size(), 2);
    BOOST_CHECK_EQUAL(combinator.combine_unary('d').size(), 0);
    BOOST_CHECK_EQUAL(combinator.combine_binary('a', 'Y').size(), 1);
    BOOST_CHECK_EQUAL(combinator.combine_binary('c', 'Y').size(), 3);
    BOOST_CHECK_EQUAL(combinator.combine_binary('b', 'Y').size(), 0);

}


BOOST_AUTO_TEST_SUITE_END()
