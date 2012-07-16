#ifndef RELOCATE_EXPRESSION_TPL_HDR
#define RELOCATE_EXPRESSION_TPL_HDR

#include "compiled_expression.tpl"
#include "opcodes.hpp"

template<class T, class S>
void relocate_expression(
    compiled_expression<T,S,2>& expr,
    int begin_attr_ix,
    int end_attr_ix)
{
    assert(end_attr_ix >= begin_attr_ix);

    for(int i = 0; i < expr.first_available_address(); ++i)
    {
    if(expr.get_opcode(i) == gobio_opcodes::OPCODE_PUSH_SUBVAR ||
       expr.get_opcode(i) == gobio_opcodes::OPCODE_SETTOP_SUBVAR ||
       expr.get_opcode(i) == gobio_opcodes::OPCODE_PUSH_EXTRA_SUBVAR ||
       expr.get_opcode(i) == gobio_opcodes::OPCODE_SETTOP_EXTRA_SUBVAR)
    {
        const int attr_ix = expr.get_iarg(i, 1);

        if(attr_ix >= begin_attr_ix && attr_ix < end_attr_ix)
        {
        if(expr.get_opcode(i) == gobio_opcodes::OPCODE_PUSH_SUBVAR
           || expr.get_opcode(i) == gobio_opcodes::OPCODE_PUSH_EXTRA_SUBVAR)
            expr.set_opcode(i, gobio_opcodes::OPCODE_PUSH_FALSE);
        else
        {
            assert(expr.get_opcode(i) == gobio_opcodes::OPCODE_SETTOP_SUBVAR
               || expr.get_opcode(i) == gobio_opcodes::OPCODE_SETTOP_EXTRA_SUBVAR);
            expr.set_opcode(i, gobio_opcodes::OPCODE_ALWAYS_FALSE);
        }
        }
        else if(attr_ix >= end_attr_ix)
        expr.set_iarg(i, 1, attr_ix - (end_attr_ix - begin_attr_ix));
    }
    }
}

template<class T, class S>
void reindex_attributes(
    compiled_expression<T,S,2>& expr,
    int symbol_ix,
    int new_symbol_ix,
    HashWrapper<int,int>::type & r_hash,
    int opcode_push_subvar,
    int opcode_settop_subvar)
{
    for(int i = 0; i < expr.first_available_address(); ++i)
    {
    if((expr.get_opcode(i) == opcode_push_subvar ||
        expr.get_opcode(i) == opcode_settop_subvar) &&
       expr.get_iarg(i, 1) == symbol_ix &&
       r_hash.count(expr.get_iarg(i, 0)))
    {
        expr.set_iarg(i, 0, r_hash[expr.get_iarg(i, 0)]);
        expr.set_iarg(i, 1, new_symbol_ix);
    }
    }
}

template<class T, class S>
void relocate_attributes_from_symbol_to_symbol(
    compiled_expression<T,S,2>& expr,
    int symbol_ix,
    int new_symbol_ix,
    int opcode_push_subvar,
    int opcode_settop_subvar)
{
    for(int i = 0; i < expr.first_available_address(); ++i)
    {
    if((expr.get_opcode(i) == opcode_push_subvar ||
        expr.get_opcode(i) == opcode_settop_subvar) &&
       expr.get_iarg(i, 1) == symbol_ix)
        expr.set_iarg(i, 1, new_symbol_ix);
    }
}

template<class T, class S>
void relocate_jumps(
    compiled_expression<T,S,2>& expr,
    int start_ip,
    int delta)
{
    for(int i = start_ip; i < expr.first_available_address(); ++i)
    {
    switch(expr.get_opcode(i))
    {
    case gobio_opcodes::OPCODE_IFJUMP:
    case gobio_opcodes::OPCODE_IFNJUMP:
    case gobio_opcodes::OPCODE_JUMP:
        expr.set_iarg(i, 0, expr.get_iarg(i, 0) + delta);
        break;

    case gobio_opcodes::OPCODE_ADD:
    case gobio_opcodes::OPCODE_ADD_CONST:
    case gobio_opcodes::OPCODE_ALWAYS_FALSE:
    case gobio_opcodes::OPCODE_ALWAYS_TRUE:
    case gobio_opcodes::OPCODE_ASSIGN:
    case gobio_opcodes::OPCODE_ASSIGN_CONST:
    case gobio_opcodes::OPCODE_EQUAL:
    case gobio_opcodes::OPCODE_EQUAL_CONST:
    case gobio_opcodes::OPCODE_HOOKING:
    case gobio_opcodes::OPCODE_HOOKING_CONST:
    case gobio_opcodes::OPCODE_NOP:
    case gobio_opcodes::OPCODE_NOT:
    case gobio_opcodes::OPCODE_NOT_EQUAL:
    case gobio_opcodes::OPCODE_NOT_EQUAL_CONST:
    case gobio_opcodes::OPCODE_POP:
    case gobio_opcodes::OPCODE_PRINT:
    case gobio_opcodes::OPCODE_PUSH:
    case gobio_opcodes::OPCODE_PUSH_FALSE:
    case gobio_opcodes::OPCODE_PUSH_SUBVAR:
    case gobio_opcodes::OPCODE_PUSH_TRUE:
    case gobio_opcodes::OPCODE_PUSH_VAR:
    case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION:
    case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION_CONST:
    case gobio_opcodes::OPCODE_SETTOP:
    case gobio_opcodes::OPCODE_SETTOP_SUBVAR:
    case gobio_opcodes::OPCODE_SETTOP_VAR:
    case gobio_opcodes::OPCODE_SET_SCORE:
    case gobio_opcodes::OPCODE_STOP:
    case gobio_opcodes::OPCODE_TREE_CHOICE:
    case gobio_opcodes::OPCODE_TRI_SEMANTICS_INTERSECTION:
    case gobio_opcodes::OPCODE_UASSIGN:
    case gobio_opcodes::OPCODE_UASSIGN_CONST:
    case gobio_opcodes::OPCODE_UEQUAL:
    case gobio_opcodes::OPCODE_UEQUAL_CONST:
        break;
    default:
        assert(0);
    }
    }
}

template<class T, class S>
bool find_loops(const compiled_expression<T,S,2>& expr)
{
    for(int i = 0; i < expr.first_available_address(); ++i)
    {
    switch(expr.get_opcode(i))
    {
    case gobio_opcodes::OPCODE_IFJUMP:
    case gobio_opcodes::OPCODE_IFNJUMP:
    case gobio_opcodes::OPCODE_JUMP:
        if(expr.get_iarg(i, 0) <= i)
        return true;
        break;

    case gobio_opcodes::OPCODE_ADD:
    case gobio_opcodes::OPCODE_ADD_CONST:
    case gobio_opcodes::OPCODE_ALWAYS_FALSE:
    case gobio_opcodes::OPCODE_ALWAYS_TRUE:
    case gobio_opcodes::OPCODE_ASSIGN:
    case gobio_opcodes::OPCODE_ASSIGN_CONST:
    case gobio_opcodes::OPCODE_EQUAL:
    case gobio_opcodes::OPCODE_EQUAL_CONST:
    case gobio_opcodes::OPCODE_EXTRA_ASSIGN:
    case gobio_opcodes::OPCODE_EXTRA_ASSIGN_CONST:
    case gobio_opcodes::OPCODE_EXTRA_UASSIGN:
    case gobio_opcodes::OPCODE_EXTRA_UASSIGN_CONST:
    case gobio_opcodes::OPCODE_HOOKING:
    case gobio_opcodes::OPCODE_HOOKING_CONST:
    case gobio_opcodes::OPCODE_NOP:
    case gobio_opcodes::OPCODE_NOT:
    case gobio_opcodes::OPCODE_NOT_EQUAL:
    case gobio_opcodes::OPCODE_NOT_EQUAL_CONST:
    case gobio_opcodes::OPCODE_POP:
    case gobio_opcodes::OPCODE_PRINT:
    case gobio_opcodes::OPCODE_PUSH:
    case gobio_opcodes::OPCODE_PUSH_EXTRA_VAR:
    case gobio_opcodes::OPCODE_PUSH_EXTRA_SUBVAR:
    case gobio_opcodes::OPCODE_PUSH_FALSE:
    case gobio_opcodes::OPCODE_PUSH_SUBVAR:
    case gobio_opcodes::OPCODE_PUSH_TRUE:
    case gobio_opcodes::OPCODE_PUSH_VAR:
    case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION:
    case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION_CONST:
    case gobio_opcodes::OPCODE_SETTOP:
    case gobio_opcodes::OPCODE_SETTOP_EXTRA_VAR:
    case gobio_opcodes::OPCODE_SETTOP_EXTRA_SUBVAR:
    case gobio_opcodes::OPCODE_SETTOP_SUBVAR:
    case gobio_opcodes::OPCODE_SETTOP_VAR:
    case gobio_opcodes::OPCODE_SET_SCORE:
    case gobio_opcodes::OPCODE_STOP:
    case gobio_opcodes::OPCODE_TREE_CHOICE:
    case gobio_opcodes::OPCODE_TRI_SEMANTICS_INTERSECTION:
    case gobio_opcodes::OPCODE_UASSIGN:
    case gobio_opcodes::OPCODE_UASSIGN_CONST:
    case gobio_opcodes::OPCODE_UEQUAL:
    case gobio_opcodes::OPCODE_UEQUAL_CONST:
        break;
    default:
        assert(0);
    }
    }

    return false;
}

template<class T, class S>
bool find_extras(const compiled_expression<T,S,2>& expr)
{
    for(int i = 0; i < expr.first_available_address(); ++i)
    {
    switch(expr.get_opcode(i))
    {
    case gobio_opcodes::OPCODE_EXTRA_ASSIGN:
    case gobio_opcodes::OPCODE_EXTRA_ASSIGN_CONST:
    case gobio_opcodes::OPCODE_EXTRA_UASSIGN:
    case gobio_opcodes::OPCODE_EXTRA_UASSIGN_CONST:
    case gobio_opcodes::OPCODE_PUSH_EXTRA_VAR:
    case gobio_opcodes::OPCODE_PUSH_EXTRA_SUBVAR:
    case gobio_opcodes::OPCODE_SETTOP_EXTRA_VAR:
    case gobio_opcodes::OPCODE_SETTOP_EXTRA_SUBVAR:
        return true;

    case gobio_opcodes::OPCODE_ADD:
    case gobio_opcodes::OPCODE_ADD_CONST:
    case gobio_opcodes::OPCODE_ALWAYS_FALSE:
    case gobio_opcodes::OPCODE_ALWAYS_TRUE:
    case gobio_opcodes::OPCODE_ASSIGN:
    case gobio_opcodes::OPCODE_ASSIGN_CONST:
    case gobio_opcodes::OPCODE_EQUAL:
    case gobio_opcodes::OPCODE_EQUAL_CONST:
    case gobio_opcodes::OPCODE_HOOKING:
    case gobio_opcodes::OPCODE_HOOKING_CONST:
    case gobio_opcodes::OPCODE_IFJUMP:
    case gobio_opcodes::OPCODE_IFNJUMP:
    case gobio_opcodes::OPCODE_JUMP:
    case gobio_opcodes::OPCODE_NOP:
    case gobio_opcodes::OPCODE_NOT:
    case gobio_opcodes::OPCODE_NOT_EQUAL:
    case gobio_opcodes::OPCODE_NOT_EQUAL_CONST:
    case gobio_opcodes::OPCODE_POP:
    case gobio_opcodes::OPCODE_PRINT:
    case gobio_opcodes::OPCODE_PUSH:
    case gobio_opcodes::OPCODE_PUSH_FALSE:
    case gobio_opcodes::OPCODE_PUSH_SUBVAR:
    case gobio_opcodes::OPCODE_PUSH_TRUE:
    case gobio_opcodes::OPCODE_PUSH_VAR:
    case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION:
    case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION_CONST:
    case gobio_opcodes::OPCODE_SETTOP:
    case gobio_opcodes::OPCODE_SETTOP_SUBVAR:
    case gobio_opcodes::OPCODE_SETTOP_VAR:
    case gobio_opcodes::OPCODE_SET_SCORE:
    case gobio_opcodes::OPCODE_STOP:
    case gobio_opcodes::OPCODE_TREE_CHOICE:
    case gobio_opcodes::OPCODE_TRI_SEMANTICS_INTERSECTION:
    case gobio_opcodes::OPCODE_UASSIGN:
    case gobio_opcodes::OPCODE_UASSIGN_CONST:
    case gobio_opcodes::OPCODE_UEQUAL:
    case gobio_opcodes::OPCODE_UEQUAL_CONST:
        break;
    default:
        assert(0);
    }
    }

    return false;
}

template<class T, class S>
bool find_non_extras_assignments(const compiled_expression<T,S,2>& expr)
{
    for(int i = 0; i < expr.first_available_address(); ++i)
    {
    switch(expr.get_opcode(i))
    {
    case gobio_opcodes::OPCODE_ASSIGN:
    case gobio_opcodes::OPCODE_ASSIGN_CONST:
    case gobio_opcodes::OPCODE_UASSIGN:
    case gobio_opcodes::OPCODE_UASSIGN_CONST:
        return true;

    case gobio_opcodes::OPCODE_ADD:
    case gobio_opcodes::OPCODE_ADD_CONST:
    case gobio_opcodes::OPCODE_ALWAYS_FALSE:
    case gobio_opcodes::OPCODE_ALWAYS_TRUE:
    case gobio_opcodes::OPCODE_EQUAL:
    case gobio_opcodes::OPCODE_EQUAL_CONST:
    case gobio_opcodes::OPCODE_EXTRA_ASSIGN:
    case gobio_opcodes::OPCODE_EXTRA_ASSIGN_CONST:
    case gobio_opcodes::OPCODE_EXTRA_UASSIGN:
    case gobio_opcodes::OPCODE_EXTRA_UASSIGN_CONST:
    case gobio_opcodes::OPCODE_HOOKING:
    case gobio_opcodes::OPCODE_HOOKING_CONST:
    case gobio_opcodes::OPCODE_IFJUMP:
    case gobio_opcodes::OPCODE_IFNJUMP:
    case gobio_opcodes::OPCODE_JUMP:
    case gobio_opcodes::OPCODE_NOP:
    case gobio_opcodes::OPCODE_NOT:
    case gobio_opcodes::OPCODE_NOT_EQUAL:
    case gobio_opcodes::OPCODE_NOT_EQUAL_CONST:
    case gobio_opcodes::OPCODE_POP:
    case gobio_opcodes::OPCODE_PRINT:
    case gobio_opcodes::OPCODE_PUSH:
    case gobio_opcodes::OPCODE_PUSH_EXTRA_SUBVAR:
    case gobio_opcodes::OPCODE_PUSH_EXTRA_VAR:
    case gobio_opcodes::OPCODE_PUSH_FALSE:
    case gobio_opcodes::OPCODE_PUSH_SUBVAR:
    case gobio_opcodes::OPCODE_PUSH_TRUE:
    case gobio_opcodes::OPCODE_PUSH_VAR:
    case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION:
    case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION_CONST:
    case gobio_opcodes::OPCODE_SETTOP:
    case gobio_opcodes::OPCODE_SETTOP_EXTRA_SUBVAR:
    case gobio_opcodes::OPCODE_SETTOP_EXTRA_VAR:
    case gobio_opcodes::OPCODE_SETTOP_SUBVAR:
    case gobio_opcodes::OPCODE_SETTOP_VAR:
    case gobio_opcodes::OPCODE_SET_SCORE:
    case gobio_opcodes::OPCODE_STOP:
    case gobio_opcodes::OPCODE_TREE_CHOICE:
    case gobio_opcodes::OPCODE_TRI_SEMANTICS_INTERSECTION:
    case gobio_opcodes::OPCODE_UEQUAL:
    case gobio_opcodes::OPCODE_UEQUAL_CONST:
    break;

    default:
        assert(0);
    }
    }

    return false;
}

template<class T, class S>
bool find_badindexes(const compiled_expression<T,S,2>& expr, size_t nb_right_symbols)
{
    for(int i = 0; i < expr.first_available_address(); ++i)
    {
    switch(expr.get_opcode(i))
    {
    case gobio_opcodes::OPCODE_IFJUMP:
    case gobio_opcodes::OPCODE_IFNJUMP:
    case gobio_opcodes::OPCODE_JUMP:
        if(expr.get_iarg(i, 0) >= expr.first_available_address())
        return true;
        break;

    case gobio_opcodes::OPCODE_PUSH_EXTRA_SUBVAR:
    case gobio_opcodes::OPCODE_PUSH_SUBVAR:
    case gobio_opcodes::OPCODE_SETTOP_EXTRA_SUBVAR:
    case gobio_opcodes::OPCODE_SETTOP_SUBVAR:
        if(expr.get_iarg(i, 1) >= nb_right_symbols ||
           expr.get_iarg(i, 1) < 0)
        return true;
        break;

    case gobio_opcodes::OPCODE_ADD:
    case gobio_opcodes::OPCODE_ADD_CONST:
    case gobio_opcodes::OPCODE_ALWAYS_FALSE:
    case gobio_opcodes::OPCODE_ALWAYS_TRUE:
    case gobio_opcodes::OPCODE_ASSIGN:
    case gobio_opcodes::OPCODE_ASSIGN_CONST:
    case gobio_opcodes::OPCODE_EQUAL:
    case gobio_opcodes::OPCODE_EQUAL_CONST:
    case gobio_opcodes::OPCODE_EXTRA_ASSIGN:
    case gobio_opcodes::OPCODE_EXTRA_ASSIGN_CONST:
    case gobio_opcodes::OPCODE_EXTRA_UASSIGN:
    case gobio_opcodes::OPCODE_EXTRA_UASSIGN_CONST:
    case gobio_opcodes::OPCODE_HOOKING:
    case gobio_opcodes::OPCODE_HOOKING_CONST:
    case gobio_opcodes::OPCODE_NOP:
    case gobio_opcodes::OPCODE_NOT:
    case gobio_opcodes::OPCODE_NOT_EQUAL:
    case gobio_opcodes::OPCODE_NOT_EQUAL_CONST:
    case gobio_opcodes::OPCODE_POP:
    case gobio_opcodes::OPCODE_PRINT:
    case gobio_opcodes::OPCODE_PUSH:
    case gobio_opcodes::OPCODE_PUSH_EXTRA_VAR:
    case gobio_opcodes::OPCODE_PUSH_FALSE:
    case gobio_opcodes::OPCODE_PUSH_TRUE:
    case gobio_opcodes::OPCODE_PUSH_VAR:
    case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION:
    case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION_CONST:
    case gobio_opcodes::OPCODE_SETTOP:
    case gobio_opcodes::OPCODE_SETTOP_EXTRA_VAR:
    case gobio_opcodes::OPCODE_SETTOP_VAR:
    case gobio_opcodes::OPCODE_SET_SCORE:
    case gobio_opcodes::OPCODE_STOP:
    case gobio_opcodes::OPCODE_TREE_CHOICE:
    case gobio_opcodes::OPCODE_TRI_SEMANTICS_INTERSECTION:
    case gobio_opcodes::OPCODE_UASSIGN:
    case gobio_opcodes::OPCODE_UASSIGN_CONST:
    case gobio_opcodes::OPCODE_UEQUAL:
    case gobio_opcodes::OPCODE_UEQUAL_CONST:
        break;
    default:
        assert(0);
    }
    }

    return false;
}


template<class T, class S>
bool is_extra_independent_on_symbol(
    boost::shared_ptr<compiled_expression<T,S,2> > expr,
    int symbol_ix,
    int starred_ix)
{
    assert(symbol_ix >= 0);
    assert(starred_ix >= -1L);

    if(symbol_ix == starred_ix)
    return false;

    if(!expr)
    return true;

    for(int i = 0; i < expr->first_available_address(); ++i)
    {
    switch(expr->get_opcode(i))
    {
    case gobio_opcodes::OPCODE_PUSH_EXTRA_SUBVAR:
    case gobio_opcodes::OPCODE_SETTOP_EXTRA_SUBVAR:
        if(expr->get_iarg(i, 1) == symbol_ix)
        return false;
        break;

    case gobio_opcodes::OPCODE_ADD:
    case gobio_opcodes::OPCODE_ADD_CONST:
    case gobio_opcodes::OPCODE_ALWAYS_FALSE:
    case gobio_opcodes::OPCODE_ALWAYS_TRUE:
    case gobio_opcodes::OPCODE_ASSIGN:
    case gobio_opcodes::OPCODE_ASSIGN_CONST:
    case gobio_opcodes::OPCODE_EQUAL:
    case gobio_opcodes::OPCODE_EQUAL_CONST:
    case gobio_opcodes::OPCODE_EXTRA_ASSIGN:
    case gobio_opcodes::OPCODE_EXTRA_ASSIGN_CONST:
    case gobio_opcodes::OPCODE_EXTRA_UASSIGN:
    case gobio_opcodes::OPCODE_EXTRA_UASSIGN_CONST:
    case gobio_opcodes::OPCODE_HOOKING:
    case gobio_opcodes::OPCODE_HOOKING_CONST:
    case gobio_opcodes::OPCODE_IFJUMP:
    case gobio_opcodes::OPCODE_IFNJUMP:
    case gobio_opcodes::OPCODE_JUMP:
    case gobio_opcodes::OPCODE_NOP:
    case gobio_opcodes::OPCODE_NOT:
    case gobio_opcodes::OPCODE_NOT_EQUAL:
    case gobio_opcodes::OPCODE_NOT_EQUAL_CONST:
    case gobio_opcodes::OPCODE_POP:
    case gobio_opcodes::OPCODE_PRINT:
    case gobio_opcodes::OPCODE_PUSH:
    case gobio_opcodes::OPCODE_PUSH_EXTRA_VAR:
    case gobio_opcodes::OPCODE_PUSH_FALSE:
    case gobio_opcodes::OPCODE_PUSH_SUBVAR:
    case gobio_opcodes::OPCODE_PUSH_TRUE:
    case gobio_opcodes::OPCODE_PUSH_VAR:
    case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION:
    case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION_CONST:
    case gobio_opcodes::OPCODE_SETTOP:
    case gobio_opcodes::OPCODE_SETTOP_EXTRA_VAR:
    case gobio_opcodes::OPCODE_SETTOP_SUBVAR:
    case gobio_opcodes::OPCODE_SETTOP_VAR:
    case gobio_opcodes::OPCODE_SET_SCORE:
    case gobio_opcodes::OPCODE_STOP:
    case gobio_opcodes::OPCODE_TREE_CHOICE:
    case gobio_opcodes::OPCODE_TRI_SEMANTICS_INTERSECTION:
    case gobio_opcodes::OPCODE_UASSIGN:
    case gobio_opcodes::OPCODE_UASSIGN_CONST:
    case gobio_opcodes::OPCODE_UEQUAL:
    case gobio_opcodes::OPCODE_UEQUAL_CONST:
        break;
    default:
        assert(0);
    }
    }

    return true;
}

template<class T, class S>
std::vector<bool> get_independence_vector(
    boost::shared_ptr<compiled_expression<T,S,2> > expr,
    size_t rhs_size,
    int starred_ix)
{
    assert(starred_ix == -1L ||
       (starred_ix >= 0 && starred_ix < (signed int)rhs_size));

    std::vector<bool> r;

    for(int i = 0; i < (signed int)rhs_size; ++i)
    r.push_back(
        is_extra_independent_on_symbol<T,S>(
        expr,
        i,
        starred_ix));
    assert(r.size() == rhs_size);

    return r;
}

template<class T, class S>
boost::shared_ptr<compiled_expression<T,S,2> >
paste_expressions(
    const compiled_expression<T,S,2>& first_expr,
    const compiled_expression<T,S,2>& second_expr)
{
    boost::shared_ptr<compiled_expression<T,S,2> > r_expr(
    new compiled_expression<T,S,2>(first_expr));

    r_expr->append(second_expr);

    relocate_jumps<T,S>(
    *r_expr,
    first_expr.first_available_address(),
    first_expr.first_available_address());

    return r_expr;
}

template<class T, class S>
boost::shared_ptr<compiled_expression<T,S,2> >
paste_expressions_with_ifnjump(
    const compiled_expression<T,S,2>& first_expr,
    const compiled_expression<T,S,2>& second_expr)
{
    compiled_expression<T,S,2> expr_with_ifnjump(first_expr);

    expr_with_ifnjump.push_instruction_1i(
    gobio_opcodes::OPCODE_IFNJUMP,
    first_expr.first_available_address() + second_expr.first_available_address());

    return paste_expressions(expr_with_ifnjump, second_expr);
}

template<class T, class S>
boost::shared_ptr<compiled_expression<T,S,2> >
push2settop(const compiled_expression<T,S,2>& expr)
{
    boost::shared_ptr<compiled_expression<T,S,2> > r_expr(
    new compiled_expression<T,S,2>(expr));

    if(r_expr->first_available_address() > 0)
    {
    switch(r_expr->get_opcode(0))
    {
    case gobio_opcodes::OPCODE_PUSH:
        r_expr->set_opcode(0, gobio_opcodes::OPCODE_SETTOP);
        break;
    case gobio_opcodes::OPCODE_PUSH_EXTRA_SUBVAR:
        r_expr->set_opcode(0, gobio_opcodes::OPCODE_SETTOP_EXTRA_SUBVAR);
        break;
    case gobio_opcodes::OPCODE_PUSH_EXTRA_VAR:
        r_expr->set_opcode(0, gobio_opcodes::OPCODE_SETTOP_EXTRA_VAR);
        break;
    case gobio_opcodes::OPCODE_PUSH_FALSE:
        r_expr->set_opcode(0, gobio_opcodes::OPCODE_ALWAYS_FALSE);
        break;
    case gobio_opcodes::OPCODE_PUSH_SUBVAR:
        r_expr->set_opcode(0, gobio_opcodes::OPCODE_SETTOP_SUBVAR);
        break;
    case gobio_opcodes::OPCODE_PUSH_TRUE:
        r_expr->set_opcode(0, gobio_opcodes::OPCODE_ALWAYS_TRUE);
        break;
    case gobio_opcodes::OPCODE_PUSH_VAR:
        r_expr->set_opcode(0, gobio_opcodes::OPCODE_SETTOP_VAR);
        break;

    case gobio_opcodes::OPCODE_ADD:
    case gobio_opcodes::OPCODE_ADD_CONST:
    case gobio_opcodes::OPCODE_ALWAYS_FALSE:
    case gobio_opcodes::OPCODE_ALWAYS_TRUE:
    case gobio_opcodes::OPCODE_ASSIGN:
    case gobio_opcodes::OPCODE_ASSIGN_CONST:
    case gobio_opcodes::OPCODE_EQUAL:
    case gobio_opcodes::OPCODE_EQUAL_CONST:
    case gobio_opcodes::OPCODE_EXTRA_ASSIGN:
    case gobio_opcodes::OPCODE_EXTRA_ASSIGN_CONST:
    case gobio_opcodes::OPCODE_EXTRA_UASSIGN:
    case gobio_opcodes::OPCODE_EXTRA_UASSIGN_CONST:
    case gobio_opcodes::OPCODE_HOOKING:
    case gobio_opcodes::OPCODE_HOOKING_CONST:
    case gobio_opcodes::OPCODE_IFJUMP:
    case gobio_opcodes::OPCODE_IFNJUMP:
    case gobio_opcodes::OPCODE_JUMP:
    case gobio_opcodes::OPCODE_NOP:
    case gobio_opcodes::OPCODE_NOT:
    case gobio_opcodes::OPCODE_NOT_EQUAL:
    case gobio_opcodes::OPCODE_NOT_EQUAL_CONST:
    case gobio_opcodes::OPCODE_POP:
    case gobio_opcodes::OPCODE_PRINT:
    case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION:
    case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION_CONST:
    case gobio_opcodes::OPCODE_SETTOP:
    case gobio_opcodes::OPCODE_SETTOP_EXTRA_SUBVAR:
    case gobio_opcodes::OPCODE_SETTOP_EXTRA_VAR:
    case gobio_opcodes::OPCODE_SETTOP_SUBVAR:
    case gobio_opcodes::OPCODE_SETTOP_VAR:
    case gobio_opcodes::OPCODE_SET_SCORE:
    case gobio_opcodes::OPCODE_STOP:
    case gobio_opcodes::OPCODE_TREE_CHOICE:
    case gobio_opcodes::OPCODE_TRI_SEMANTICS_INTERSECTION:
    case gobio_opcodes::OPCODE_UASSIGN:
    case gobio_opcodes::OPCODE_UASSIGN_CONST:
    case gobio_opcodes::OPCODE_UEQUAL:
    case gobio_opcodes::OPCODE_UEQUAL_CONST:
        break;
    default:
        assert(0);
    }
    }

    return r_expr;
}

template<class T, class S>
boost::shared_ptr<compiled_expression<T,S,2> >
settop2push(const compiled_expression<T,S,2>& expr)
{
    boost::shared_ptr<compiled_expression<T,S,2> > r_expr(
    new compiled_expression<T,S,2>(expr));

    if(r_expr->first_available_address() > 0)
    {
    switch(r_expr->get_opcode(0))
    {
    case gobio_opcodes::OPCODE_ALWAYS_FALSE:
        r_expr->set_opcode(0, gobio_opcodes::OPCODE_PUSH_FALSE);
        break;
    case gobio_opcodes::OPCODE_ALWAYS_TRUE:
        r_expr->set_opcode(0, gobio_opcodes::OPCODE_PUSH_TRUE);
        break;
    case gobio_opcodes::OPCODE_SETTOP:
        r_expr->set_opcode(0, gobio_opcodes::OPCODE_SETTOP);
        break;
    case gobio_opcodes::OPCODE_SETTOP_EXTRA_SUBVAR:
        r_expr->set_opcode(0, gobio_opcodes::OPCODE_PUSH_EXTRA_SUBVAR);
        break;
    case gobio_opcodes::OPCODE_SETTOP_EXTRA_VAR:
        r_expr->set_opcode(0, gobio_opcodes::OPCODE_PUSH_EXTRA_VAR);
        break;
    case gobio_opcodes::OPCODE_SETTOP_SUBVAR:
        r_expr->set_opcode(0, gobio_opcodes::OPCODE_PUSH_SUBVAR);
        break;
    case gobio_opcodes::OPCODE_SETTOP_VAR:
        r_expr->set_opcode(0, gobio_opcodes::OPCODE_PUSH_VAR);
        break;

    case gobio_opcodes::OPCODE_ADD:
    case gobio_opcodes::OPCODE_ADD_CONST:
    case gobio_opcodes::OPCODE_ASSIGN:
    case gobio_opcodes::OPCODE_ASSIGN_CONST:
    case gobio_opcodes::OPCODE_EQUAL:
    case gobio_opcodes::OPCODE_EQUAL_CONST:
    case gobio_opcodes::OPCODE_EXTRA_ASSIGN:
    case gobio_opcodes::OPCODE_EXTRA_ASSIGN_CONST:
    case gobio_opcodes::OPCODE_EXTRA_UASSIGN:
    case gobio_opcodes::OPCODE_EXTRA_UASSIGN_CONST:
    case gobio_opcodes::OPCODE_HOOKING:
    case gobio_opcodes::OPCODE_HOOKING_CONST:
    case gobio_opcodes::OPCODE_IFJUMP:
    case gobio_opcodes::OPCODE_IFNJUMP:
    case gobio_opcodes::OPCODE_JUMP:
    case gobio_opcodes::OPCODE_NOP:
    case gobio_opcodes::OPCODE_NOT:
    case gobio_opcodes::OPCODE_NOT_EQUAL:
    case gobio_opcodes::OPCODE_NOT_EQUAL_CONST:
    case gobio_opcodes::OPCODE_POP:
    case gobio_opcodes::OPCODE_PRINT:
    case gobio_opcodes::OPCODE_PUSH:
    case gobio_opcodes::OPCODE_PUSH_EXTRA_SUBVAR:
    case gobio_opcodes::OPCODE_PUSH_EXTRA_VAR:
    case gobio_opcodes::OPCODE_PUSH_FALSE:
    case gobio_opcodes::OPCODE_PUSH_SUBVAR:
    case gobio_opcodes::OPCODE_PUSH_TRUE:
    case gobio_opcodes::OPCODE_PUSH_VAR:
    case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION:
    case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION_CONST:
    case gobio_opcodes::OPCODE_SET_SCORE:
    case gobio_opcodes::OPCODE_STOP:
    case gobio_opcodes::OPCODE_TREE_CHOICE:
    case gobio_opcodes::OPCODE_TRI_SEMANTICS_INTERSECTION:
    case gobio_opcodes::OPCODE_UASSIGN:
    case gobio_opcodes::OPCODE_UASSIGN_CONST:
    case gobio_opcodes::OPCODE_UEQUAL:
    case gobio_opcodes::OPCODE_UEQUAL_CONST:
        break;
    default:
        assert(0);
    }
    }

    return r_expr;
}




#endif
