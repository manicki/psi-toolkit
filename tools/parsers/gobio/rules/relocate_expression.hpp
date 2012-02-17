#ifndef RELOCATE_EXPRESSION_HPP_HDR
#define RELOCATE_EXPRESSION_HPP_HDR

template<class T, class S>
void relocate_expression(
    compiled_expression<T,S,2>& expr,
    int begin_attr_ix,
    int end_attr_ix);

template<class T, class S>
void reindex_attributes(
    compiled_expression<T,S,2>& expr,
    int symbol_ix,
    int new_symbol_ix,
    hash_wrapper<int,int>::type & r_hash,
    int opcode_push_subvar,
    int opcode_settop_subvar);

template<class T, class S>
void relocate_attributes_from_symbol_to_symbol(
    compiled_expression<T,S,2>& expr,
    int symbol_ix,
    int new_symbol_ix,
    int opcode_push_subvar,
    int opcode_settop_subvar);

template<class T, class S>
void relocate_jumps(
    compiled_expression<T,S,2>& expr,
    int start_ip,
    int delta);

template<class T, class S>
bool find_loops(const compiled_expression<T,S,2>& expr);

template<class T, class S>
bool find_extras(const compiled_expression<T,S,2>& expr);

template<class T, class S>
bool find_non_extras_assignments(const compiled_expression<T,S,2>& expr);

template<class T, class S>
bool find_badindexes(const compiled_expression<T,S,2>& expr, size_t nb_right_symbols);

    
template<class T, class S>
bool is_extra_independent_on_symbol(
    boost::shared_ptr<compiled_expression<T,S,2> > expr,
    int symbol_ix,
    int starred_ix);

template<class T, class S>
std::vector<bool> get_independence_vector(
    boost::shared_ptr<compiled_expression<T,S,2> > expr,
    size_t rhs_size,
    int starred_ix);

template<class T, class S>
boost::shared_ptr<compiled_expression<T,S,2> >
paste_expressions(
    const compiled_expression<T,S,2>& first_expr,
    const compiled_expression<T,S,2>& second_expr);

template<class T, class S>
boost::shared_ptr<compiled_expression<T,S,2> >
paste_expressions_with_ifnjump(
    const compiled_expression<T,S,2>& first_expr,
    const compiled_expression<T,S,2>& second_expr);

template<class T, class S>
boost::shared_ptr<compiled_expression<T,S,2> >
push2settop(const compiled_expression<T,S,2>& expr);

template<class T, class S>
boost::shared_ptr<compiled_expression<T,S,2> >
settop2push(const compiled_expression<T,S,2>& expr);


#endif
