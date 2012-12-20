#ifndef EXTRACT_TREE_HPP_HDR
#define EXTRACT_TREE_HPP_HDR

#include <boost/shared_ptr.hpp>
#include <vector>
#include "tree_branch.tpl"
#include "av_matrix.hpp"
#include <iostream>
#include <string>

template<class T, class Ch, class K>
boost::shared_ptr<tree_branch<T,Ch,typename K::equivalent_type> > extract_tree_branch(
    Ch& chart,
    typename Ch::edge_descriptor edge,
    K& combinator,
    const std::vector<typename K::rule_holder>& local_rules);

template<class T, class Ch, class K>
void show_tree(
    Ch& chart,
    typename Ch::edge_descriptor edge,
    K& combinator,
    const std::vector<typename K::rule_holder>& local_rules,
    std::ostream& ostr);


#endif
