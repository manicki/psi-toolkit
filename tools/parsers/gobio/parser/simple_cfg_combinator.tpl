#ifndef SIMPLE_CFG_COMBINATOR_TPL_HDR
#define SIMPLE_CFG_COMBINATOR_TPL_HDR

#include "simple_cfg_combinator.hpp"

//DEBUG
//#include <iostream>

template<typename T, typename R>
void simple_cfg_combinator<T,R>::add_unary_rule(T rhs, R rule)
{
    unary_rules_.push_back(unary_rule_entry(rhs, rule));
}

template<typename T, typename R>
void simple_cfg_combinator<T,R>::add_binary_rule(T first_rhs, T second_rhs, R rule)
{
    binary_rules_.push_back(binary_rule_entry(first_rhs,
                          second_rhs, rule));
}

template<typename T, typename R>
std::vector<R> simple_cfg_combinator<T,R>::combine_unary(T symbol)
{
    std::vector<R> rules_found;

    typename std::list<unary_rule_entry>::iterator iter = unary_rules_.begin();
    for( ; iter != unary_rules_.end(); ++iter)
    if((*iter).rhs == symbol)
        rules_found.push_back((*iter).rule);

    return rules_found;
}

template<typename T, typename R>
std::vector<R> simple_cfg_combinator<T,R>::combine_binary(T first_symbol, T second_symbol)
{
    std::vector<R> rules_found;

    typename std::list<binary_rule_entry>::iterator iter = binary_rules_.begin();
    for( ; iter != binary_rules_.end(); ++iter)
    if((*iter).first_rhs == first_symbol && (*iter).second_rhs == second_symbol)
        rules_found.push_back((*iter).rule);

    return rules_found;
}

template<typename T, typename R>
std::vector<T> simple_cfg_combinator<T,R>::find_left_sibling(R parent_rule, T second_symbol)
{
    std::vector<T> left_siblings_found;

    typename std::list<binary_rule_entry>::iterator iter = binary_rules_.begin();
    for( ; iter != binary_rules_.end(); ++iter)
    if((*iter).rule == parent_rule && (*iter).second_rhs == second_symbol)
        left_siblings_found.push_back((*iter).first_rhs);

    return left_siblings_found;
}

template<typename T, typename R>
std::vector<T> simple_cfg_combinator<T,R>::find_right_sibling(R parent_rule, T first_symbol)
{
    std::vector<T> right_siblings_found;

    typename std::list<binary_rule_entry>::iterator iter = binary_rules_.begin();
    for( ; iter != binary_rules_.end(); ++iter)
    if((*iter).rule == parent_rule && (*iter).first_rhs == first_symbol)
        right_siblings_found.push_back((*iter).second_rhs);

    return right_siblings_found;
}

template<typename T, typename R>
std::vector<R> simple_cfg_combinator<T,R>::find_only_child_parent(T symbol)
{
    std::vector<R> parents_found;

    typename std::list<unary_rule_entry>::iterator iter = unary_rules_.begin();
    for( ; iter != unary_rules_.end(); ++iter) {
//DEBUG
//std::cout << (*iter).rhs << " = " << symbol << " : ";
//std::cout << ((*iter).rhs == symbol) << std::endl;
        if((*iter).rhs == symbol) {
            parents_found.push_back((*iter).rule);
        }
    }

    return parents_found;
}

template<typename T, typename R>
std::vector< std::pair<R,T> > simple_cfg_combinator<T,R>::find_left_sibling_with_parent(T second_symbol)
{
    std::vector<std::pair<R,T> > families_found;

    typename std::list<binary_rule_entry>::iterator iter = binary_rules_.begin();
    for( ; iter != binary_rules_.end(); ++iter)
    if((*iter).second_rhs == second_symbol)
        families_found.push_back(std::pair<R,T>((*iter).rule,(*iter).first_rhs));

    return families_found;
}

template<typename T, typename R>
std::vector<std::pair<R,T> > simple_cfg_combinator<T,R>::find_right_sibling_with_parent(T first_symbol)
{
    std::vector<std::pair<R,T> > families_found;

    typename std::list<binary_rule_entry>::iterator iter = binary_rules_.begin();
    for( ; iter != binary_rules_.end(); ++iter)
    if((*iter).first_rhs == first_symbol)
        families_found.push_back(std::pair<R,T>((*iter).rule,(*iter).second_rhs));

    return families_found;
}

template<typename T, typename R>
std::vector< std::pair< R, std::pair<T,T> > > simple_cfg_combinator<T,R>::split(T parent_symbol)
{
    std::vector< std::pair< R, std::pair<T,T> > > rules_found;

    typename std::list<binary_rule_entry>::iterator iter = binary_rules_.begin();
    for( ; iter != binary_rules_.end(); ++iter)
    if((*iter).rule.category() == parent_symbol)
        rules_found.push_back(
            std::pair< R, std::pair<T,T> >(
                (*iter).rule,
                std::pair<T,T>((*iter).first_rhs, (*iter).second_rhs)
            )
        );

    return rules_found;
}

template<typename T, typename R>
std::vector< std::pair< R,T > > simple_cfg_combinator<T,R>::find_only_children(T parent_symbol)
{
    std::vector< std::pair< R,T > > rules_found;

    typename std::list<unary_rule_entry>::iterator iter = unary_rules_.begin();
    for( ; iter != unary_rules_.end(); ++iter)
    if((*iter).rule.category() == parent_symbol)
        rules_found.push_back(
            std::pair< R,T >(
                (*iter).rule,
                (*iter).rhs
            )
        );

    return rules_found;
}

template<typename T, typename R>
void simple_cfg_combinator<T,R>::set_root(T state) {
    if (!is_root(state)) {
        roots_.push_back(state);
    }
}

template<typename T, typename R>
bool simple_cfg_combinator<T,R>::is_root(T state) {
    typename std::list<T>::iterator iter = roots_.begin();
    for( ; iter != roots_.end(); ++iter)
    if(*iter == state) {
        return true;
    }
    return false;
}

// przenioslem do sx.hpp
/*
template<typename T, typename R>
void simple_cfg_combinator<T,R>::mark_as_used(T state) {
    if (!is_used(state)) {
        used_.push_back(state);
    }
}

template<typename T, typename R>
bool simple_cfg_combinator<T,R>::is_used(T state) {
    typename std::list<T>::iterator iter = used_.begin();
    for( ; iter != used_.end(); ++iter)
    if(*iter == state) {
        return true;
    }
}

template<typename T, typename R>
void simple_cfg_combinator<T,R>::clear_used() {
    used_.clear();
}
*/

#endif
