#ifndef CONVERT_TREE_RECIPE_HPP_HDR
#define CONVERT_TREE_RECIPE_HPP_HDR

#include "grules.h"
#include "registrar.tpl"
#include "tree_specification.hpp"

#include <boost/shared_ptr.hpp>

template<class T, class M>
boost::shared_ptr<tree_specification<T> > convert_tree_recipe(
    const GRuleTreeRecipe* recipe,
    HashWrapper<std::pair<int,int>, int>::type& rhs_hash,
    M& master,
    registrar<std::string>& symbol_registrar,
    int starred_ix);

#endif
