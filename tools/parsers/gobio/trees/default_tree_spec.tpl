#ifndef DEFAULT_TREE_SPEC_TPL_HDR
#define DEFAULT_TREE_SPEC_TPL_HDR

#include "default_tree_spec.hpp"

template<class T, class M>
boost::shared_ptr<tree_specification<T> > default_tree_spec(
    size_t nb_symbols,
    int starred_ix,
    M& master,
    bool set_default_head)
{
    assert(starred_ix == -1L
       || (starred_ix >= 0 && starred_ix < (signed int)nb_symbols));

    if(starred_ix == -1L && set_default_head)
    starred_ix = 0;

    boost::shared_ptr<tree_specification<T> > spec;

    spec.reset(new tree_specification<T>(
           starred_ix == -1L
           ? boost::shared_ptr<tree_specification_root<T> >(new empty_root<T>())
           : boost::shared_ptr<tree_specification_root<T> >(
               new hook_root<T>(starred_ix))));

    for(int li = 0; li < starred_ix; ++li)
    spec->add_left_subspec(
        boost::shared_ptr<tree_specification<T> >(
        new tree_specification<T>(
            boost::shared_ptr<tree_specification_root<T> >(
            new hook_root<T>(li)))),
        master.false_value());

    for(int ri = starred_ix+1; ri < (signed int)nb_symbols; ++ri)
    spec->add_right_subspec(
        boost::shared_ptr<tree_specification<T> >(
        new tree_specification<T>(
            boost::shared_ptr<tree_specification_root<T> >(
            new hook_root<T>(ri)))),
        master.false_value());


    return spec;
}

#endif
