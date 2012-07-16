#ifndef TREE_SPECIFICATION_TPL_HDR
#define TREE_SPECIFICATION_TPL_HDR

#include "tree_specification.hpp"
#include <algorithm>
#include <sstream>

template<class T>
tree_specification<T>::tree_specification(
    boost::shared_ptr<tree_specification_root<T> > root):
    root_(root)
{
}

template<class T>
boost::shared_ptr<tree_specification_root<T> > tree_specification<T>::root()
{
    return root_;
}

template<class T>
size_t tree_specification<T>::nb_subspecs() const
{
    return nb_left_subspecs() + nb_right_subspecs();
}

template<class T>
size_t tree_specification<T>::nb_left_subspecs() const
{
    return left_subspecs_.size();
}

template<class T>
size_t tree_specification<T>::nb_right_subspecs() const
{
    return right_subspecs_.size();
}

template<class T>
boost::shared_ptr<tree_specification<T> > tree_specification<T>::subspec(size_t i)
{
    assert(i < nb_subspecs());

    if(i >= nb_left_subspecs())
    return right_subspec(i - nb_left_subspecs());

    return left_subspec(i);
}

template<class T>
T tree_specification<T>::subspec_label(size_t i) const
{
    assert(i < nb_subspecs());

    if(i >= nb_left_subspecs())
    return right_subspec_label(i - nb_left_subspecs());

    return left_subspec_label(i);
}

template<class T>
boost::shared_ptr<tree_specification<T> > tree_specification<T>::left_subspec(size_t i)
{
    assert(i < nb_left_subspecs());

    return left_subspecs_[i].spec;
}

template<class T>
T tree_specification<T>::left_subspec_label(size_t i) const
{
    assert(i < nb_left_subspecs());

    return left_subspecs_[i].label;
}


template<class T>
boost::shared_ptr<tree_specification<T> > tree_specification<T>::right_subspec(size_t i)
{
    assert(i < nb_right_subspecs());

    return right_subspecs_[i].spec;
}

template<class T>
T tree_specification<T>::right_subspec_label(size_t i) const
{
    assert(i < nb_right_subspecs());

    return right_subspecs_[i].label;
}

template<class T>
void tree_specification<T>::add_left_subspec(boost::shared_ptr<tree_specification> subspec, T label)
{
    assert(subspec);

    left_subspecs_.push_back(spec_link(subspec, label));

    assert(nb_left_subspecs() > 0);
}

template<class T>
void tree_specification<T>::add_right_subspec(boost::shared_ptr<tree_specification> subspec, T label)
{
    assert(subspec);

    right_subspecs_.push_back(spec_link(subspec, label));

    assert(nb_right_subspecs() > 0);
}

template<class T>
void tree_specification<T>::relocate(size_t begin_symbol_ix, int delta)
{
    root_->relocate(begin_symbol_ix, delta);

    for(typename std::vector<typename tree_specification<T>::spec_link>::iterator lit
        = left_subspecs_.begin();
    lit != left_subspecs_.end();
    ++lit)
    (*lit).spec->relocate(begin_symbol_ix, delta);

    for(typename std::vector<typename tree_specification<T>::spec_link>::iterator rit
        = right_subspecs_.begin();
    rit != right_subspecs_.end();
    ++rit)
    (*rit).spec->relocate(begin_symbol_ix, delta);

    for(typename std::vector<typename tree_specification<T>::insert>::iterator iit
        = inserts_.begin();
    iit != inserts_.end();
    ++iit)
    (*iit).spec->relocate(begin_symbol_ix, delta);

}


template<class T>
bool tree_specification<T>::remove_hooks(size_t begin_symbol_ix, size_t end_symbol_ix)
{
    bool all_removed = true;

    if(root_->remove_hook(begin_symbol_ix, end_symbol_ix))
    root_.reset(new empty_root<T>());
    else
    all_removed = false;

    left_subspecs_.erase(
    std::remove_if(
        left_subspecs_.begin(),
        left_subspecs_.end(),
        remove_hooks_predicate(begin_symbol_ix, end_symbol_ix)),
    left_subspecs_.end());

    right_subspecs_.erase(
    std::remove_if(
        right_subspecs_.begin(),
        right_subspecs_.end(),
        remove_hooks_predicate(begin_symbol_ix, end_symbol_ix)),
    right_subspecs_.end());

    inserts_.erase(
    std::remove_if(
        inserts_.begin(),
        inserts_.end(),
        remove_hooks_predicate_for_inserts(begin_symbol_ix, end_symbol_ix)),
    inserts_.end());

    if(left_subspecs_.size() > 0 ||
       right_subspecs_.size() > 0 ||
       inserts_.size() > 0)
       all_removed = false;

    return all_removed;
}

template<class T>
void tree_specification<T>::move_hook_down(
    size_t symbol_ix, size_t new_symbol_ix, size_t sub_symbol_ix)
{
    root_->move_hook_down(symbol_ix, new_symbol_ix, sub_symbol_ix);

    std::for_each(
    left_subspecs_.begin(),
    left_subspecs_.end(),
    move_hook_down_functor(symbol_ix, new_symbol_ix, sub_symbol_ix));

    std::for_each(
    right_subspecs_.begin(),
    right_subspecs_.end(),
    move_hook_down_functor(symbol_ix, new_symbol_ix, sub_symbol_ix));

    std::for_each(
    inserts_.begin(),
    inserts_.end(),
    move_hook_down_functor_for_inserts(symbol_ix, new_symbol_ix, sub_symbol_ix));
}


template<class T>
size_t tree_specification<T>::nb_inserts() const
{
    return inserts_.size();
}

template<class T>
boost::shared_ptr<tree_specification<T> > tree_specification<T>::ins_spec(size_t insert_ix)
{
    assert(insert_ix < inserts_.size());

    return inserts_[insert_ix].spec;
}

template<class T>
T tree_specification<T>::ins_label(size_t insert_ix) const
{
    assert(insert_ix < inserts_.size());

    return inserts_[insert_ix].label;
}


template<class T>
size_t tree_specification<T>::ins_path_length(size_t insert_ix) const
{
    assert(insert_ix < inserts_.size());

    return inserts_[insert_ix].path.size();
}

template<class T>
T tree_specification<T>::ins_path_category(size_t insert_ix, size_t element_ix) const
{
    assert(insert_ix < inserts_.size());
    assert(element_ix < ins_path_length(insert_ix));

    return inserts_[insert_ix].path[element_ix].category;
}

template<class T>
T tree_specification<T>::ins_path_label(size_t insert_ix, size_t element_ix) const
{
    assert(insert_ix < inserts_.size());
    assert(element_ix < ins_path_length(insert_ix));

    return inserts_[insert_ix].path[element_ix].label;
}

template<class T>
void tree_specification<T>::add_insert(
    boost::shared_ptr<tree_specification> a_spec,
    T a_label,
    T p_category,
    T p_label)
{
    assert(a_spec);

    inserts_.push_back(typename tree_specification<T>::insert
               (a_spec, a_label, p_category, p_label));
}

template<class T>
void tree_specification<T>::add_path_element(
    size_t insert_ix,
    T category,
    T label)
{
    assert(insert_ix < inserts_.size());

    inserts_[insert_ix].path.push_back(
    typename tree_specification<T>::insert::path_element(category, label));
}

template<class T>
boost::shared_ptr<tree_specification<T> > tree_specification<T>::clone() const
{
    boost::shared_ptr<tree_specification<T> > cloned_spec(
    new tree_specification<T>(root_->clone()));

    cloned_spec->left_subspecs_.reserve(left_subspecs_.size());
    cloned_spec->right_subspecs_.reserve(right_subspecs_.size());
    cloned_spec->inserts_.reserve(inserts_.size());

    for(size_t i = 0; i < left_subspecs_.size(); ++i)
    cloned_spec->left_subspecs_.push_back(
        typename tree_specification<T>::spec_link(
        left_subspecs_[i].spec->clone(),
        left_subspecs_[i].label));

    for(size_t j = 0; j < right_subspecs_.size(); ++j)
    cloned_spec->right_subspecs_.push_back(
        typename tree_specification<T>::spec_link(
        right_subspecs_[j].spec->clone(),
        right_subspecs_[j].label));

    for(size_t k = 0; k < inserts_.size(); ++k)
    {
    cloned_spec->inserts_.push_back(
        typename tree_specification<T>::insert(
        inserts_[k].spec->clone(),
        inserts_[k].label));

    cloned_spec->inserts_.back().path = inserts_[k].path;
    }

    return cloned_spec;
}

template<class T> template<class Ch, class Sel>
void tree_specification<T>::ground(
    Ch& chart, typename Ch::partition_iterator partition_it, Sel& selector)
{

}


template<class T> template<class M>
std::string tree_specification<T>::string_representation(M& master) const
{
    master_string_representationer<T,M> msrer(master);

    std::string r = root_->string_representation(msrer);

    std::string subs;

    for(size_t li = 0; li < left_subspecs_.size(); ++li)
    subs += std::string(subs.length() ? "," : "")
        + left_subspecs_[li].spec->string_representation(master)
        + (master.is_false(left_subspecs_[li].label)
           ? std::string()
           : (std::string(":")
          + master.string_representation(left_subspecs_[li].label)))
        + "+";


    for(size_t li = 0; li < right_subspecs_.size(); ++li)
    subs += std::string(subs.length() ? ",+" : "+")
        + right_subspecs_[li].spec->string_representation(master)
        + (master.is_false(right_subspecs_[li].label)
           ? std::string()
           : (std::string(":")
          + master.string_representation(right_subspecs_[li].label)));

    for(size_t ii = 0; ii < inserts_.size(); ++ii)
    {
    std::string p;
    for(size_t pi = 0; pi < inserts_[ii].path.size();++pi)
        p += std::string("/")
        + (master.is_false(inserts_[ii].path[pi].category)
           ? std::string()
           : master.string_representation(inserts_[ii].path[pi].category))
        + (master.is_false(inserts_[ii].path[pi].label)
           ? std::string()
           : (std::string(":")
              + master.string_representation(inserts_[ii].path[pi].label)));

    subs += std::string(subs.length() > 0 ? "," : "") + p;

    subs += inserts_[ii].spec->string_representation(master);

    if(master.is_true(inserts_[ii].label))
        subs += std::string(":")
        + master.string_representation(inserts_[ii].label);
    }


    if(subs.length() > 0)
    r += std::string("[") + subs + std::string("]");

    return r;
}


template<class T>
category_root<T>::category_root(T category):category_(category)
{
}

template<class T>
boost::shared_ptr<tree_specification_root<T> > category_root<T>::clone() const
{
    return boost::shared_ptr<tree_specification_root<T> >(new category_root<T>(*this));
}

template<class T>
std::string category_root<T>::string_representation(string_representationer<T>& srer) const
{
    return srer.string_representation(category_);
}


template<class T>
hook_root<T>::hook_root(size_t symbol_ix)
{
    symbol_ixs_.push_back(symbol_ix);
}

template<class T>
boost::shared_ptr<tree_specification_root<T> > hook_root<T>::clone() const
{
    return boost::shared_ptr<tree_specification_root<T> >(new hook_root<T>(*this));
}

template<class T>
void hook_root<T>::relocate(size_t begin_symbol_ix, int delta)
{
    for(size_t i = 0; i < symbol_ixs_.size(); ++i)
    if(symbol_ixs_[i] >= begin_symbol_ix)
        symbol_ixs_[i] += delta;
}

template<class T>
bool hook_root<T>::remove_hook(size_t begin_symbol_ix, size_t end_symbol_ix)
{
    for(size_t i = 0; i < symbol_ixs_.size(); ++i)
    if(symbol_ixs_[i] >= begin_symbol_ix
       && symbol_ixs_[i] < end_symbol_ix)
        return true;

    return false;
}

template<class T>
void hook_root<T>::move_hook_down(
    size_t symbol_ix, size_t new_symbol_ix, size_t sub_symbol_ix)
{
    assert(symbol_ixs_.size() > 0);

    if(symbol_ixs_[0] == symbol_ix)
    {
    symbol_ixs_.insert(symbol_ixs_.begin() + 1, sub_symbol_ix);
    symbol_ixs_[0] = new_symbol_ix;

    assert(symbol_ixs_[1] == sub_symbol_ix);
    }
}


template<class T>
bool hook_root<T>::is_hook(size_t symbol_ix)
{
    assert(symbol_ixs_.size() > 0);

    return symbol_ixs_[0] == symbol_ix;
}

template<class T>
std::vector<size_t> hook_root<T>::get_hook_path() const
{
    return symbol_ixs_;
}


template<class T>
std::string hook_root<T>::string_representation(string_representationer<T>&) const
{
    std::ostringstream osstr;

    for(size_t i = 0; i < symbol_ixs_.size(); ++i)
    osstr << '#' << symbol_ixs_[i];

    return osstr.str();
}


template<class T>
hook_contents_root<T>::hook_contents_root(size_t symbol_ix)
{
    symbol_ixs_.push_back(symbol_ix);
}

template<class T>
boost::shared_ptr<tree_specification_root<T> > hook_contents_root<T>::clone() const
{
    return boost::shared_ptr<tree_specification_root<T> >(new hook_contents_root<T>(*this));
}

template<class T>
void hook_contents_root<T>::relocate(size_t begin_symbol_ix, int delta)
{
    for(size_t i = 0; i < symbol_ixs_.size(); ++i)
    if(symbol_ixs_[i] >= begin_symbol_ix)
        symbol_ixs_[i] += delta;
}

template<class T>
bool hook_contents_root<T>::remove_hook(size_t begin_symbol_ix, size_t end_symbol_ix)
{
    for(size_t i = 0; i < symbol_ixs_.size(); ++i)
    if(symbol_ixs_[i] >= begin_symbol_ix
       && symbol_ixs_[i] < end_symbol_ix)
        return true;

    return false;
}

template<class T>
void hook_contents_root<T>::move_hook_down(
    size_t symbol_ix, size_t new_symbol_ix, size_t sub_symbol_ix)
{
    assert(symbol_ixs_.size() > 0);

    if(symbol_ixs_[0] == symbol_ix)
    {
    symbol_ixs_.insert(symbol_ixs_.begin() + 1, sub_symbol_ix);
    symbol_ixs_[0] = new_symbol_ix;

    assert(symbol_ixs_[1] == sub_symbol_ix);
    }
}

template<class T>
std::vector<size_t> hook_contents_root<T>::get_hook_path() const
{
    return symbol_ixs_;
}


template<class T>
std::string hook_contents_root<T>::string_representation(string_representationer<T>&) const
{
    std::ostringstream osstr;

    osstr << '*';

    for(size_t i = 0; i < symbol_ixs_.size(); ++i)
    osstr << '#' << symbol_ixs_[i];

    return osstr.str();
}


template<class T> template<class Archive>
void tree_specification<T>::serialize(Archive & ar, const unsigned int /*version*/)
{
    ar & root_;
    ar & left_subspecs_;
    ar & right_subspecs_;
    ar & inserts_;
}

#endif
