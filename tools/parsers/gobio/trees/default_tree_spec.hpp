#ifndef DEFAULT_TREE_SPEC_HPP_HDR
#define DEFAULT_TREE_SPEC_HPP_HDR

#include "tree_specification.hpp"

#include <boost/shared_ptr.hpp>


template<class T, class M>
boost::shared_ptr<tree_specification<T> > default_tree_spec(
    size_t nb_symbols,
    int starred_ix,
    M& master,
    bool set_default_head=false);

#endif
