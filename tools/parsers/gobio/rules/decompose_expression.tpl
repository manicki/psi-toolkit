#ifndef DECOMPOSE_EXPRESSION_HPP_HDR
#define DECOMPOSE_EXPRESSION_HPP_HDR

#include "decompose_expression.hpp"
#include "relocate_expression.tpl"

#include <iostream>

template<class T, class S>
void stack_behaviour(
    int op_code,
    size_t& nb_popped_not_peeked,
    size_t& nb_popped_peeked,
    size_t& nb_pushed,
    bool& side_effects,
    bool& is_jump,
    bool& is_subvar_ref,
    bool& is_assign,
    bool& is_var_ref)
{

#ifdef STACK_BEHAVIOUR_ENTRY
#error "STACK_BEHAVIOUR_ENTRY already defined"
#endif

#define STACK_BEHAVIOUR_ENTRY(opcode_, popped_not_peeked_, popped_peeked_, pushed_, side_effects_, is_jump_, is_subvar_ref_, is_assign_, is_var_ref_) \
    case gobio_opcodes:: opcode_: \
    nb_popped_not_peeked = popped_not_peeked_; \
    nb_popped_peeked = popped_peeked_; \
    nb_pushed = pushed_; \
    side_effects = side_effects_; \
    is_jump = is_jump_; \
    is_subvar_ref = is_subvar_ref_; \
    is_assign = is_assign_; \
    is_var_ref = is_var_ref_; \
    break

    switch(op_code)
    {
    STACK_BEHAVIOUR_ENTRY(OPCODE_ADD, 0, 2, 1, false, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_ADD_CONST, 0, 1, 1, false, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_ALWAYS_FALSE, 1, 0, 1, false, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_ALWAYS_TRUE, 1, 0, 1, false, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_ASSIGN, 0, 1, 1, true, false, false, true, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_ASSIGN_CONST, 0, 0, 1, true, false, false, true, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_EQUAL, 0, 2, 1, false, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_EQUAL_CONST, 0, 1, 1, false, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_EXTRA_ASSIGN, 0, 1, 1, true, false, false, true, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_EXTRA_ASSIGN_CONST, 0, 0, 1, true, false, false, true, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_EXTRA_UASSIGN, 0, 1, 1, true, false, false, true, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_EXTRA_UASSIGN_CONST, 0, 0, 1, true, false, false, true, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_HOOKING, 0, 2, 1, false, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_HOOKING_CONST, 0, 1, 1, false, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_IFJUMP, 0, 1, 1, false, true, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_IFNJUMP, 0, 1, 1, false, true, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_JUMP, 0, 0, 0, false, true, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_NOP, 0, 0, 0, false, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_NOT, 0, 1, 1, false, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_NOT_EQUAL, 0, 2, 1, false, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_NOT_EQUAL_CONST, 0, 1, 1, false, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_POP, 1, 0, 0, false, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_PRINT, 0, 1, 1, true, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_PUSH, 0, 0, 1, false, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_PUSH_EXTRA_SUBVAR, 0, 0, 1, false, false, true, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_PUSH_EXTRA_VAR, 0, 0, 1, false, false, false, false, true);
    STACK_BEHAVIOUR_ENTRY(OPCODE_PUSH_FALSE, 0, 0, 1, false, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_PUSH_SUBVAR, 0, 0, 1, false, false, true, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_PUSH_TRUE, 0, 0, 1, false, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_PUSH_VAR, 0, 0, 1, false, false, false, false, true);
    STACK_BEHAVIOUR_ENTRY(OPCODE_SEMANTICS_INTERSECTION, 0, 2, 1, false, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_SEMANTICS_INTERSECTION_CONST, 0, 2, 1, false, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_SETTOP, 1, 0, 1, false, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_SETTOP_EXTRA_SUBVAR, 1, 0, 1, false, false, true, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_SETTOP_EXTRA_VAR, 1, 0, 1, false, false, false, false, true);
    STACK_BEHAVIOUR_ENTRY(OPCODE_SETTOP_SUBVAR, 1, 0, 1, false, false, true, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_SETTOP_VAR, 1, 0, 1, false, false, false, false, true);
    STACK_BEHAVIOUR_ENTRY(OPCODE_SET_SCORE, 0, 0, 1, true, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_STOP, 1, 0, 0, true, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_TREE_CHOICE, 0, 0, 1, true, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_TRI_SEMANTICS_INTERSECTION, 0, 3, 1, false, false, false, false, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_UASSIGN, 0, 1, 1, true, false, false, true, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_UASSIGN_CONST, 0, 0, 1, true, false, false, true, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_UEQUAL, 0, 2, 1, false, false, false, true, false);
    STACK_BEHAVIOUR_ENTRY(OPCODE_UEQUAL_CONST, 0, 1, 1, false, false, false, true, false);
    default:
    assert(0);
    }

#undef STACK_BEHAVIOUR_ENTRY
}

template<class T, class S>
void empty_block(
    boost::shared_ptr<compiled_expression<T, S, 2> > expr,
    int start_ix,
    int cut_point_ix)
{
    assert(start_ix >= 0);
    assert(cut_point_ix > start_ix);

    assert(cut_point_ix < expr->first_available_address());

    for (int i = start_ix; i < cut_point_ix - 1; ++i)
    expr->set_opcode(i, gobio_opcodes::OPCODE_NOP, true);

    expr->set_opcode(
    cut_point_ix - 1,
    (start_ix == 0
     ? gobio_opcodes::OPCODE_PUSH_TRUE
     : gobio_opcodes::OPCODE_ALWAYS_TRUE),
    true);
}

template<class T, class S>
void denopify(boost::shared_ptr<compiled_expression<T, S, 2> > expr)
{
    assert(expr);

    int start_nop_seq = -1L;

    for (int i = 0; i < expr->first_available_address(); ++i)
    {
    if (expr->get_opcode(i) == gobio_opcodes::OPCODE_NOP)
    {
        if (start_nop_seq == -1L)
        start_nop_seq = i;
    }
    else
    {
        if (start_nop_seq >= 0)
        {
        // przesuwamy

        int delta = i - start_nop_seq;

        for (int j = 0; j < expr->first_available_address(); ++j)
            if ((expr->get_opcode(j) == gobio_opcodes::OPCODE_IFNJUMP
            || expr->get_opcode(j) == gobio_opcodes::OPCODE_IFJUMP
            || expr->get_opcode(j) == gobio_opcodes::OPCODE_JUMP)
               && expr->get_iarg(j, 0) >= i)
            expr->set_iarg(j, 0, expr->get_iarg(j, 0) - delta);

        expr->delete_instructions(start_nop_seq, i);

        i -= delta;
        start_nop_seq = -1L;
        }
    }
    }
}

template<class T, class S>
void detruify(boost::shared_ptr<compiled_expression<T, S, 2> > expr)
{
    assert(expr);

    bool prev_always_true = false;

    for (int i = 0; i < expr->first_available_address(); ++i)
    {
    if (prev_always_true
       && expr->get_opcode(i) == gobio_opcodes::OPCODE_ALWAYS_TRUE)
        expr->set_opcode(i, gobio_opcodes::OPCODE_NOP, true);
    else
    {
        prev_always_true =
        (expr->get_opcode(i) == gobio_opcodes::OPCODE_PUSH_TRUE ||
         expr->get_opcode(i) == gobio_opcodes::OPCODE_ALWAYS_TRUE ||
         expr->get_opcode(i) == gobio_opcodes::OPCODE_SET_SCORE ||
         expr->get_opcode(i) == gobio_opcodes::OPCODE_TREE_CHOICE ||
         expr->get_opcode(i) == gobio_opcodes::OPCODE_IFNJUMP);
    }
    }
}

template<class T, class S>
void deifnjumpify(boost::shared_ptr<compiled_expression<T, S, 2> > expr)
{
    assert(expr);

    if (expr->first_available_address() >= 2)
    {
    assert(expr->get_opcode(expr->first_available_address() - 1)
           == gobio_opcodes::OPCODE_STOP);

    int penult = expr->first_available_address() - 2;

    if (expr->get_opcode(penult)
       == gobio_opcodes::OPCODE_IFNJUMP)
    {
        assert(expr->get_iarg(penult,0) == penult + 1);
        expr->set_opcode(penult, gobio_opcodes::OPCODE_NOP, true);
    }
    }
}

template<class T, class S>
void depushtruify(boost::shared_ptr<compiled_expression<T, S, 2> > expr)
{
    assert(expr);

    if (expr->first_available_address() >= 2)
    {
    if (expr->get_opcode(0) == gobio_opcodes::OPCODE_PUSH_TRUE)
    {
        if (expr->get_opcode(1) == gobio_opcodes::OPCODE_SETTOP_VAR)
        {
        expr->set_opcode(0, gobio_opcodes::OPCODE_NOP);
        expr->set_opcode(1, gobio_opcodes::OPCODE_PUSH_VAR);
        }
        else if (expr->get_opcode(1) == gobio_opcodes::OPCODE_SETTOP_SUBVAR)
        {
        expr->set_opcode(0, gobio_opcodes::OPCODE_NOP);
        expr->set_opcode(1, gobio_opcodes::OPCODE_PUSH_SUBVAR);
        }
    }
    }
}


template<class T, class S>
boost::shared_ptr<compiled_expression<T, S, 2> > decompose_expression(
    boost::shared_ptr<compiled_expression<T, S, 2> > expr,
    size_t nb_symbols,
    int starred_ix,
    std::vector<boost::shared_ptr<compiled_expression<T, S, 2> > >&  parts)
{
    boost::shared_ptr<compiled_expression<T, S, 2> > new_expr
    = boost::shared_ptr<compiled_expression<T, S, 2> >(
        new compiled_expression<T, S, 2>(*expr));

    parts.resize(nb_symbols);

    int prev_block_cut_point = 0;
    int stack_elements_at_play = 0;

    int last_address = expr->first_available_address() - 1;

    bool block_side_effects = false;
    std::vector<bool> block_used_subvars;
    std::vector<bool> set_attrs;
    block_used_subvars.resize(nb_symbols);
    int min_cut_point = -1L;

    for (int i = 0; i <= last_address; ++i)
    {
    size_t nb_popped_not_peeked = 0;
    size_t nb_popped_peeked = 0;
    size_t nb_pushed = 0;
    bool side_effects = false;
    bool is_jump = false;
    bool is_subvar_ref = false;
    bool is_assign = false;
    bool is_var_ref = false;

    stack_behaviour<T,S>(
        expr->get_opcode(i),
        nb_popped_not_peeked,
        nb_popped_peeked,
        nb_pushed,
        side_effects,
        is_jump,
        is_subvar_ref,
        is_assign,
        is_var_ref);

    if (is_jump
       && !(expr->get_iarg(i, 0) == last_address
        &&  expr->get_opcode(i) == gobio_opcodes::OPCODE_IFNJUMP)
       && expr->get_iarg(i, 0) > min_cut_point)
        min_cut_point = expr->get_iarg(i, 0);

    assert(nb_popped_not_peeked <= 1);
    assert(nb_popped_peeked <= 3);
    assert(nb_pushed <= 1);

    if (stack_elements_at_play > 0
       && stack_elements_at_play - nb_popped_not_peeked <= 0
       && i >= min_cut_point)
    {
        assert(stack_elements_at_play == (signed int)nb_popped_not_peeked);

        stack_elements_at_play = 0;

        int block_cut_point = i;

        // mamy blok, analizujemy go

        size_t nb_used_subvars = 0;
        int choosen_ix = -1L;
        for (size_t i = 0; i < block_used_subvars.size(); ++i)
        if (block_used_subvars[i])
        {
            choosen_ix = (signed int)i;
            ++nb_used_subvars;
        }

        if (nb_used_subvars == 1 && !block_side_effects)
        {
        assert(choosen_ix >= 0);

        empty_block(new_expr, prev_block_cut_point, block_cut_point);

        if (!parts[choosen_ix])
        {
            parts[choosen_ix].reset(
            new compiled_expression<T, S, 2>(*expr));

            if (prev_block_cut_point > 0)
            empty_block(parts[choosen_ix], 0, prev_block_cut_point);
        }
        }
        else
        choosen_ix = -1L;

        for (int j = 0; j < (signed int)nb_symbols; ++j)
        if (j != choosen_ix && parts[j])
            empty_block(parts[j], prev_block_cut_point, block_cut_point);

        // sprzatamy po bloku

        block_side_effects = false;
        min_cut_point = -1L;
        prev_block_cut_point = block_cut_point;

        block_used_subvars = std::vector<bool>();
        block_used_subvars.resize(nb_symbols);
    }
    else
        stack_elements_at_play -= nb_popped_not_peeked;

    if (is_subvar_ref)
    {
        assert(expr->get_iarg(i, 1) <= (signed int)nb_symbols);
        block_used_subvars[expr->get_iarg(i, 1)] = true;
    }

    if (is_assign)
    {
        int attr_ix = expr->get_iarg(i, 0);
        set_attrs.resize(attr_ix+1);
        set_attrs[attr_ix] = true;
    }

    if (is_var_ref)
    {
        int attr_ix = expr->get_iarg(i, 0);

        if (starred_ix >= 0
           && (attr_ix >= (signed int)set_attrs.size()
           || !set_attrs[attr_ix]))
        block_used_subvars[starred_ix] = true;
        else
        side_effects = true;
    }

    if (side_effects)
        block_side_effects = true;

    stack_elements_at_play -= nb_popped_peeked;
    stack_elements_at_play += nb_pushed;
    }

    for (size_t p = 0; p < nb_symbols; ++p)
    if (parts[p])
    {
        denopify(parts[p]);
        detruify(parts[p]);
        denopify(parts[p]);
        deifnjumpify(parts[p]);
        denopify(parts[p]);
        depushtruify(parts[p]);
        denopify(parts[p]);
    }

    denopify(new_expr);
    detruify(new_expr);
    denopify(new_expr);
    deifnjumpify(new_expr);
    denopify(new_expr);
    depushtruify(new_expr);
    denopify(new_expr);

    return new_expr;
}

template<class T, class S>
std::vector<boost::shared_ptr<compiled_expression<T, S, 2> > > get_bifiltres(
    boost::shared_ptr<compiled_expression<T, S, 2> > expr,
    size_t nb_symbols,
    int starred_ix,
    int first_symbol_ix)
{
    std::vector<boost::shared_ptr<compiled_expression<T, S, 2> > > bifiltres;

    int prev_block_cut_point = 0;
    int stack_elements_at_play = 0;

    int last_address = expr->first_available_address() - 1;

    bool block_side_effects = false;
    std::vector<bool> block_used_subvars;
    std::vector<bool> set_attrs;
    block_used_subvars.resize(nb_symbols);
    int min_cut_point = -1L;

    for (int i = 0; i <= last_address; ++i)
    {
    size_t nb_popped_not_peeked = 0;
    size_t nb_popped_peeked = 0;
    size_t nb_pushed = 0;
    bool side_effects = false;
    bool is_jump = false;
    bool is_subvar_ref = false;
    bool is_assign = false;
    bool is_var_ref = false;

    stack_behaviour<T,S>(
        expr->get_opcode(i),
        nb_popped_not_peeked,
        nb_popped_peeked,
        nb_pushed,
        side_effects,
        is_jump,
        is_subvar_ref,
        is_assign,
        is_var_ref);

    if (is_jump
       && !(expr->get_iarg(i, 0) == last_address
        &&  expr->get_opcode(i) == gobio_opcodes::OPCODE_IFNJUMP)
       && expr->get_iarg(i, 0) > min_cut_point)
        min_cut_point = expr->get_iarg(i, 0);

    assert(nb_popped_not_peeked <= 1);
    assert(nb_popped_peeked <= 3);
    assert(nb_pushed <= 1);

    if (stack_elements_at_play > 0
       && stack_elements_at_play - nb_popped_not_peeked <= 0
       && i >= min_cut_point)
    {
        assert(stack_elements_at_play == (signed int)nb_popped_not_peeked);

        stack_elements_at_play = 0;

        int block_cut_point = i;

        // mamy blok, analizujemy go

        size_t nb_used_subvars = 0;
        for (size_t i = 0; i < block_used_subvars.size(); ++i)
        if (block_used_subvars[i])
            ++nb_used_subvars;

        if (nb_used_subvars == 2
           && block_used_subvars[first_symbol_ix]
           && block_used_subvars[first_symbol_ix + 1]
           && !block_side_effects)
        {
        assert(block_cut_point > prev_block_cut_point);

        boost::shared_ptr<compiled_expression<T, S, 2> > bif
            = expr->get_slice(prev_block_cut_point, block_cut_point-1);

        relocate_jumps<T,S>(
            *bif,
            0,
            -prev_block_cut_point);

        relocate_expression<T,S>(
            *bif,
            0,
            first_symbol_ix);

        bifiltres.push_back(push2settop<T,S>(*bif));
        }

        // sprzatamy po bloku

        block_side_effects = false;
        min_cut_point = -1L;
        prev_block_cut_point = block_cut_point;

        block_used_subvars = std::vector<bool>();
        block_used_subvars.resize(nb_symbols);
    }
    else
        stack_elements_at_play -= nb_popped_not_peeked;

    if (is_subvar_ref)
    {
        assert(expr->get_iarg(i, 1) <= (signed int)nb_symbols);
        block_used_subvars[expr->get_iarg(i, 1)] = true;
    }

    if (is_assign)
    {
        int attr_ix = expr->get_iarg(i, 0);
        set_attrs.resize(attr_ix+1);
        set_attrs[attr_ix] = true;
    }

    if (is_var_ref)
    {
        int attr_ix = expr->get_iarg(i, 0);

        if (starred_ix >= 0
           && (attr_ix >= (signed int)set_attrs.size()
           || !set_attrs[attr_ix]))
        block_used_subvars[starred_ix] = true;
        else
        side_effects = true;
    }

    if (side_effects)
        block_side_effects = true;

    stack_elements_at_play -= nb_popped_peeked;
    stack_elements_at_play += nb_pushed;
    }

    return bifiltres;
}



#endif

