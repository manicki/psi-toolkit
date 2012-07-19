#ifndef DECOMPOSE_EXPRESSION_HPP_HDR
#define DECOMPOSE_EXPRESSION_HPP_HDR

#include <boost/shared_ptr.hpp>
#include <vector>

template<class T, class S>
boost::shared_ptr<compiled_expression<T,S,2> > decompose_expression(
    boost::shared_ptr<compiled_expression<T,S,2> > expr,
    size_t nb_symbols,
    int starred_ix,
    std::vector<boost::shared_ptr<compiled_expression<T,S,2> > >& parts);


template<class T, class S>
std::vector<boost::shared_ptr<compiled_expression<T,S,2> > > get_bifiltres(
    boost::shared_ptr<compiled_expression<T,S,2> > expr,
    size_t nb_symbols,
    int starred_ix,
    int first_symbol_ix);

#endif
