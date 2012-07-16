#ifndef COMPILE_GRULEEXPRESSION_HPP_HDR
#define COMPILE_GRULEEXPRESSION_HPP_HDR

#include "grules.h"
#include "compiled_expression.hpp"
#include "registrar.hpp"

#include "tree_specification.hpp"

#include <vector>
#include <boost/shared_ptr.hpp>

#include "hash_wrapper.hpp"

template<class T, class S, class M>
void compile_gruleexpression(
    const GRuleExpression* original_expr,
    HashWrapper<std::pair<int,int>, int>::type& rhs_hash,
    compiled_expression<T,S,2>& compiled_expr,
    M& master,
    registrar<std::string>& attribute_registrar,
    registrar<std::string>& extra_attribute_registrar,
    registrar<std::string>& symbol_registrar,
    int starred_ix,
    std::vector<boost::shared_ptr<tree_specification<T> > >& tree_specs,
    S setscore_factor=1);




#endif
