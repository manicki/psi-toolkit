#ifndef TREE_BRANCH_TPL_HDR
#define TREE_BRANCH_TPL_HDR

#include "tree_branch.hpp"
#include "tree_specification.tpl"

template<class T, class Ch, class E>
tree_branch<T,Ch,E>::tree_branch():is_supported_(false)
{
}

template<class T, class Ch, class E>
T tree_branch<T,Ch,E>::root() const
{
    return root_;
}

template<class T, class Ch, class E>
void tree_branch<T,Ch,E>::set_root(T a_root)
{
    root_ = a_root;
}

template<class T, class Ch, class E>
typename Ch::edge_descriptor tree_branch<T,Ch,E>::supporting_edge()
{
    return edge_;
}

template<class T, class Ch, class E>
typename Ch::variant_iterator tree_branch<T,Ch,E>::supporting_variant()
{
    return variant_it_;
}

template<class T, class Ch, class E>
bool tree_branch<T,Ch,E>::is_supported()
{
    return is_supported_;
}


template<class T, class Ch, class E>
void tree_branch<T,Ch,E>::set_support(
    typename Ch::edge_descriptor e,
    typename Ch::variant_iterator vit)
{
    edge_ = e;
    variant_it_ = vit;
    is_supported_ = true;
}

template<class T, class Ch, class E>
void tree_branch<T,Ch,E>::set_equivalent(E eq)
{
    equivalent_ = eq;
}

template<class T, class Ch, class E>
E tree_branch<T,Ch,E>::equivalent()
{
    return equivalent_;
}

template<class T, class Ch, class E>
size_t tree_branch<T,Ch,E>::nb_children() const
{
    return children_.size();
}

template<class T, class Ch, class E>
T tree_branch<T,Ch,E>::child_label(size_t i) const
{
    assert(i < children_.size());

    return children_[i].label;
}

template<class T, class Ch, class E>
boost::shared_ptr<tree_specification<T> > tree_branch<T,Ch,E>::child_spec(size_t i)
{
    assert(i < children_.size());

    return children_[i].spec;
}

template<class T, class Ch, class E>
typename Ch::variant_iterator tree_branch<T,Ch,E>::child_variant_it(size_t i)
{
    assert(i < children_.size());

    return children_[i].variant_it;
}

template<class T, class Ch, class E>
typename Ch::edge_descriptor tree_branch<T,Ch,E>::child_edge(size_t i)
{
    assert(i < children_.size());

    return children_[i].edge;
}

template<class T, class Ch, class E>
void tree_branch<T,Ch,E>::add_child(
    T label,
    boost::shared_ptr<tree_specification<T> > spec,
    typename Ch::edge_descriptor edge,
    typename Ch::variant_iterator variant_it)
{
    children_.push_back(child_link(label, spec, edge, variant_it));
}

#endif
