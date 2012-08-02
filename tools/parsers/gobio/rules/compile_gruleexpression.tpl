#ifndef COMPILE_GRULEEXPRESSION_TPL_HDR
#define COMPILE_GRULEEXPRESSION_TPL_HDR

#include <hash_wrapper.hpp>

HASH_WRAPPER_HASH_INTRO

    template<>
    struct HASH_WRAPPER_HASH_TRAITS<std::pair<int, int> >
    {
        HASH_WRAPPER_EXTRA_STUFF

        size_t operator()(const std::pair<int, int>& k) const
        { return HASH_WRAPPER_HASH_TRAITS<int>()(k.first) ^ HASH_WRAPPER_HASH_TRAITS<int>()(k.second); }

#ifdef __VS__
        bool operator()(const std::pair<int, int>& a,
                const std::pair<int, int>& b) const
        {
            return a != b;
        }
#endif
    };

HASH_WRAPPER_HASH_OUTRO


#include "compile_gruleexpression.hpp"
#include "opcodes.hpp"
#include "tree_specification.tpl"
#include "convert_tree_recipe.tpl"

#include <stdexcept>



#include <vector>

#include "grules.h"

void right_symbols_to_hash(
    const GRuleRightSymbolsSeq& a_symbols_seq,
    HashWrapper<std::pair<int, int>, int>::type& right_symbols_hash,
    registrar<std::string>& symbol_registrar,
    int& current_ix,
    int& starred_ix);

void right_symbol_alts_seq_to_hash(
    const GRuleRightSymbolAltsSeq& a_symbol_alts_seq,
    HashWrapper<std::pair<int, int>, int>::type& right_symbols_hash,
    registrar<std::string>& symbol_registrar,
    int& current_ix,
    int& starred_ix);

bool is_constant(const GRuleExpression* expr);

template<class T, class M>
T expr_to_constant(
    const GRuleExpression* expr,
    M& master)
{
    switch(expr->expr_type)
    {
    case GRuleExpression::NUMBER:
    return master.from_int(expr->number);

    case GRuleExpression::ATOM:
    assert(expr->atom);
    return master.from_string(*(expr->atom));

    default:
    assert(0);
    return master.false_value();
    }
}

struct traversal_stack_item
{
    const GRuleExpression* expr;
    int address;
    bool left_visited;
    bool right_visited;
};


template<class T, class S, class M>
void compile_gruleexpression(
    const GRuleExpression* original_expr,
    HashWrapper<std::pair<int, int>, int>::type& rhs_hash,
    compiled_expression<T,S,2>& compiled_expr,
    M& master,
    registrar<std::string>& attribute_registrar,
    registrar<std::string>& extra_attribute_registrar,
    registrar<std::string>& symbol_registrar,
    int starred_ix,
    std::vector<boost::shared_ptr<tree_specification<T> > >& tree_specs,
    S setscore_factor)
{
    assert(tree_specs.size() == 1);

    if (!original_expr)
    {
    compiled_expr.push_instruction_0(gobio_opcodes::OPCODE_PUSH_TRUE);
    compiled_expr.push_instruction_0(gobio_opcodes::OPCODE_STOP);
    return;
    }

    std::vector<traversal_stack_item> traversal_stack;
    traversal_stack.reserve(16);

    int last_else_jmp_address = -1L;

    const GRuleExpression* current_expr = original_expr;

    traversal_stack.push_back(traversal_stack_item());
    traversal_stack.back().expr = current_expr;

    while (!traversal_stack.empty())
    {
    current_expr = traversal_stack.back().expr;

    bool is_assign = (current_expr->expr_type == GRuleExpression::OPERATOR_BI
              &&
              (current_expr->expr_operator == GRuleExpression::OPERATOR_ASSIGN
               || current_expr->expr_operator == GRuleExpression::OPERATOR_UASSIGN));

    if (current_expr->left_subexpr && !is_assign && !traversal_stack.back().left_visited )
    {
        traversal_stack.back().left_visited = true;

        traversal_stack.push_back(traversal_stack_item());
        traversal_stack.back().expr = current_expr->left_subexpr;
        continue;
    }

    if (current_expr->right_subexpr
       && !traversal_stack.back().right_visited)
    {
        // is_right_constant wskazuje, czy prawe wyrażenie jest stałe
        // ORAZ czy ma sens wykorzystać to w optymalizacji (np.
        // w wyrażeniach warunkowych is_right_constant nie będzie włączone
        // nawet gdy prawe wyrażenie jest stałe!)
        // będą rzadkie)
        bool is_right_constant = false;

        if (current_expr->expr_type == GRuleExpression::OPERATOR_BI)
        {
        switch(current_expr->expr_operator)
        {
        case GRuleExpression::OPERATOR_AND:

            // optymalizacja: jeśli wiemy, że ostatnia instrukcja
            // kładzie na stos wartość prawdziwą i nie ma
            // żadnego IFNJUMP do tego miejsca
            // nie umieszczamy instrukcji IFNJUMP
            if (is_top_true(compiled_expr, master)
               && !is_jump_or_ifnjump_to(
               compiled_expr, compiled_expr.first_available_address()))
            traversal_stack.back().address = -1L;
            else
            {
            traversal_stack.back().address =
                compiled_expr.first_available_address();

            compiled_expr.push_instruction_1i(
                gobio_opcodes::OPCODE_IFNJUMP,
                -1L);
            }

            compiled_expr.push_instruction_0(gobio_opcodes::OPCODE_POP);

            break;

        case GRuleExpression::OPERATOR_OR:
            traversal_stack.back().address =
            compiled_expr.first_available_address();

            compiled_expr.push_instruction_1i(
            gobio_opcodes::OPCODE_IFJUMP,
            -1L);

            compiled_expr.push_instruction_0(gobio_opcodes::OPCODE_POP);

            break;

        case GRuleExpression::OPERATOR_IMPLICATION:
            compiled_expr.push_instruction_0(gobio_opcodes::OPCODE_NOT);

            traversal_stack.back().address =
            compiled_expr.first_available_address();

            compiled_expr.push_instruction_1i(
            gobio_opcodes::OPCODE_IFJUMP,
            -1L);

            compiled_expr.push_instruction_0(gobio_opcodes::OPCODE_POP);

            break;

        case GRuleExpression::OPERATOR_ELSE:
            traversal_stack.back().address =
            compiled_expr.first_available_address();

            compiled_expr.push_instruction_1i(
            gobio_opcodes::OPCODE_JUMP,
            -1L);

            break;

        default:
            is_right_constant
            = (is_constant(current_expr->right_subexpr) &&
            !
            (current_expr->expr_operator == GRuleExpression::OPERATOR_SEMANTICS_INTERSECTION
             && current_expr->left_subexpr->expr_type == GRuleExpression::OPERATOR_BI
             && current_expr->left_subexpr->expr_operator == GRuleExpression::OPERATOR_BINARG));
        }
        }

        if (!is_right_constant)
        {
        traversal_stack.back().right_visited = true;

        traversal_stack.push_back(traversal_stack_item());
        traversal_stack.back().expr = current_expr->right_subexpr;
        continue;
        }
    }

    compile_gruleexpression_node(
        current_expr,
        rhs_hash,
        compiled_expr,
        master,
        attribute_registrar,
        extra_attribute_registrar,
        symbol_registrar,
        traversal_stack,
        last_else_jmp_address,
        starred_ix,
        tree_specs,
        setscore_factor);


    last_else_jmp_address =
        ((current_expr->expr_type == GRuleExpression::OPERATOR_BI
          && current_expr->expr_operator == GRuleExpression:: OPERATOR_ELSE) ?
         (traversal_stack.back().address + 1) :
         -1L);

    traversal_stack.pop_back();
    }

    compiled_expr.push_instruction_0(gobio_opcodes::OPCODE_STOP);
}


template<class T, class S>
bool is_top_pop(const compiled_expression<T,S,2>& compiled_expr)
{
    return
    compiled_expr.first_available_address() > 0
    && compiled_expr.get_opcode(compiled_expr.first_available_address()-1) == gobio_opcodes::OPCODE_POP;
}

template<class T, class S>
bool is_jump_or_ifnjump_to(const compiled_expression<T,S,2>& expr, int addr)
{
    for (int i = 0; i < expr.first_available_address(); ++i)
    {
    switch(expr.get_opcode(i))
    {
    case gobio_opcodes::OPCODE_IFNJUMP:
    case gobio_opcodes::OPCODE_JUMP:
        if (expr.get_iarg(i, 0) == addr)
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
    case gobio_opcodes::OPCODE_IFJUMP:
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

template<class T, class S, class M>
bool is_top_true(
    const compiled_expression<T,S,2>& compiled_expr,
    const M& master)
{
    int i = compiled_expr.first_available_address() - 1;

    // pomijamy instrukcje ASSIGN, które nie zmieniają stanu stosu
    while (i >= 0
      &&
      (compiled_expr.get_opcode(i) == gobio_opcodes::OPCODE_ASSIGN
       || compiled_expr.get_opcode(i) == gobio_opcodes::OPCODE_EXTRA_ASSIGN)) --i;

    if (i >= 0)
    {
    switch(compiled_expr.get_opcode(i))
    {
    case gobio_opcodes::OPCODE_ALWAYS_TRUE:
    case gobio_opcodes::OPCODE_PRINT:
    case gobio_opcodes::OPCODE_PUSH_TRUE:
    case gobio_opcodes::OPCODE_TREE_CHOICE:
    case gobio_opcodes::OPCODE_SET_SCORE:
        return true;

    case gobio_opcodes::OPCODE_ASSIGN_CONST:
    case gobio_opcodes::OPCODE_EXTRA_ASSIGN_CONST:
    case gobio_opcodes::OPCODE_PUSH:
    case gobio_opcodes::OPCODE_SETTOP:
        return master.is_true(compiled_expr.get_targ(i, 0));

    case gobio_opcodes::OPCODE_ADD:
    case gobio_opcodes::OPCODE_ADD_CONST:
    case gobio_opcodes::OPCODE_ALWAYS_FALSE:
    case gobio_opcodes::OPCODE_EQUAL:
    case gobio_opcodes::OPCODE_EQUAL_CONST:
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
    case gobio_opcodes::OPCODE_PUSH_FALSE:
    case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION:
    case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION_CONST:
    case gobio_opcodes::OPCODE_TRI_SEMANTICS_INTERSECTION:
    case gobio_opcodes::OPCODE_UASSIGN:
    case gobio_opcodes::OPCODE_UASSIGN_CONST:
    case gobio_opcodes::OPCODE_UEQUAL:
    case gobio_opcodes::OPCODE_UEQUAL_CONST:
    case gobio_opcodes::OPCODE_PUSH_EXTRA_SUBVAR:
    case gobio_opcodes::OPCODE_PUSH_EXTRA_VAR:
    case gobio_opcodes::OPCODE_PUSH_SUBVAR:
    case gobio_opcodes::OPCODE_PUSH_VAR:
    case gobio_opcodes::OPCODE_SETTOP_EXTRA_SUBVAR:
    case gobio_opcodes::OPCODE_SETTOP_EXTRA_VAR:
    case gobio_opcodes::OPCODE_SETTOP_SUBVAR:
    case gobio_opcodes::OPCODE_SETTOP_VAR:
        return false;

    case gobio_opcodes::OPCODE_STOP:
    case gobio_opcodes::OPCODE_ASSIGN:
    case gobio_opcodes::OPCODE_EXTRA_ASSIGN:
        assert(0);
        break;
    default:
        assert(0);
    }
    }

    return false;
}



template<class T, class S, class M>
void compile_gruleexpression_node(
    const GRuleExpression* expr,
    HashWrapper<std::pair<int, int>, int>::type& rhs_hash,
    compiled_expression<T,S,2>& compiled_expr,
    M& master,
    registrar<std::string>& attribute_registrar,
    registrar<std::string>& extra_attribute_registrar,
    registrar<std::string>& symbol_registrar,
    std::vector<traversal_stack_item>& traversal_stack,
    int last_else_jmp_address,
    int starred_ix,
    std::vector<boost::shared_ptr<tree_specification<T> > >& tree_specs,
    S setscore_factor)
{
    switch(expr->expr_type)
    {

    case GRuleExpression::NUMBER:
    // Sprawdzamy, czy poprzednią wygenerowaną instrukcją nie było POP, jeśli
    // tak zamiast sekwencji POP/PUSH generujemy pojedynczą instrukcję SETTOP
    if (is_top_pop(compiled_expr))
    {
        compiled_expr.pop_instruction();
        compiled_expr.push_instruction_1t(
        gobio_opcodes::OPCODE_SETTOP,
        master.from_int(expr->number));
    }
    else
        compiled_expr.push_instruction_1t(
        gobio_opcodes::OPCODE_PUSH,
        master.from_int(expr->number));
    break;

    case GRuleExpression::ATOM:
    assert(expr->atom);

    if (is_top_pop(compiled_expr))
    {
        compiled_expr.pop_instruction();
        compiled_expr.push_instruction_1t(
        gobio_opcodes::OPCODE_SETTOP,
        master.from_string(*(expr->atom)));
    }
    else
        compiled_expr.push_instruction_1t(
        gobio_opcodes::OPCODE_PUSH,
        master.from_string(*(expr->atom)));
    break;

    case GRuleExpression::ATTRIBUTE:
    assert(expr->attribute);

    if (!(expr->atom))
    {
        bool is_extra = extra_attribute_registrar.is_registered(
        *(expr->attribute));

        registrar<std::string>& reg =
        (is_extra
         ? extra_attribute_registrar
         : attribute_registrar);

        if (is_top_pop(compiled_expr))
        {
        compiled_expr.pop_instruction();
        compiled_expr.push_instruction_1i(
            (is_extra ? gobio_opcodes::OPCODE_SETTOP_EXTRA_VAR : gobio_opcodes::OPCODE_SETTOP_VAR),
            reg.get_id(*(expr->attribute)));
        }
        else
        compiled_expr.push_instruction_1i(
            (is_extra ? gobio_opcodes::OPCODE_PUSH_EXTRA_VAR : gobio_opcodes::OPCODE_PUSH_VAR),
            reg.get_id(*(expr->attribute)));
    }
    else
    {
        assert(expr->number >= 0);

        int symbol_id = symbol_registrar.get_id(*(expr->atom));

        if (!rhs_hash.count(
           std::pair<int, int>(
               symbol_id,
               expr->number)))
        throw std::runtime_error("unknown symbol");

        bool is_extra = extra_attribute_registrar.is_registered(
        *(expr->attribute));

        registrar<std::string>& reg =
        (is_extra
         ? extra_attribute_registrar
         : attribute_registrar);

        if (is_top_pop(compiled_expr))
        {
        compiled_expr.pop_instruction();
        compiled_expr.push_instruction_2ii(
            (is_extra ? gobio_opcodes::OPCODE_SETTOP_EXTRA_SUBVAR : gobio_opcodes::OPCODE_SETTOP_SUBVAR),
            reg.get_id(*(expr->attribute)),
            rhs_hash[
            std::pair<int, int>(
                symbol_id,
                expr->number)]);
        }
        else
        compiled_expr.push_instruction_2ii(
            (is_extra ? gobio_opcodes::OPCODE_PUSH_EXTRA_SUBVAR : gobio_opcodes::OPCODE_PUSH_SUBVAR),
            reg.get_id(*(expr->attribute)),
            rhs_hash[
            std::pair<int, int>(
                symbol_id,
                expr->number)]);
    }
    break;

    case GRuleExpression::OPERATOR_MONO:
    assert(expr->left_subexpr);
    switch(expr->expr_operator)
    {
    case GRuleExpression::OPERATOR_ALWAYS_TRUE:
        compiled_expr.push_instruction_0(gobio_opcodes::OPCODE_ALWAYS_TRUE);
        break;
    case GRuleExpression::OPERATOR_PRINT:
        compiled_expr.push_instruction_0(gobio_opcodes::OPCODE_PRINT);
        break;
    default:
        assert(0);
    }; break;

    case GRuleExpression::TREE_CHOICE:
    compiled_expr.push_instruction_1i(
        gobio_opcodes::OPCODE_TREE_CHOICE,
        tree_specs.size());

    assert(expr->tree_choice);
    tree_specs.push_back(
        convert_tree_recipe<T,M>(
        expr->tree_choice,
        rhs_hash,
        master,
        symbol_registrar,
        starred_ix));
    break;

    case GRuleExpression::SETSCORE:
    compiled_expr.push_instruction_1f(
        gobio_opcodes::OPCODE_SET_SCORE,
        expr->score * setscore_factor);
    break;

    case GRuleExpression::OPERATOR_BI:
    assert(expr->left_subexpr);
    assert(expr->right_subexpr);

    switch(expr->expr_operator)
    {
    case GRuleExpression::OPERATOR_ASSIGN:
    {
        assert(expr->left_subexpr->attribute);

        bool is_extra = extra_attribute_registrar.is_registered(
        *(expr->left_subexpr->attribute));

        registrar<std::string>& reg =
        (is_extra
         ? extra_attribute_registrar
         : attribute_registrar);

        if (is_constant(expr->right_subexpr))
        compiled_expr.push_instruction_2it(
            (is_extra ? gobio_opcodes::OPCODE_EXTRA_ASSIGN_CONST : gobio_opcodes::OPCODE_ASSIGN_CONST),
            reg.get_id(*(expr->left_subexpr->attribute)),
            expr_to_constant<T>(
            expr->right_subexpr,
            master));
        else
        compiled_expr.push_instruction_1i(
            (is_extra ? gobio_opcodes::OPCODE_EXTRA_ASSIGN : gobio_opcodes::OPCODE_ASSIGN),
            reg.get_id(*(expr->left_subexpr->attribute)));
        break;
    }

    case GRuleExpression::OPERATOR_UASSIGN:
    {
        assert(expr->left_subexpr->attribute);

        bool is_extra = extra_attribute_registrar.is_registered(
        *(expr->left_subexpr->attribute));

        registrar<std::string>& reg =
        (is_extra
         ? extra_attribute_registrar
         : attribute_registrar);

        if (is_constant(expr->right_subexpr))
        compiled_expr.push_instruction_2it(
            (is_extra ? gobio_opcodes::OPCODE_EXTRA_UASSIGN_CONST : gobio_opcodes::OPCODE_UASSIGN_CONST),
            reg.get_id(*(expr->left_subexpr->attribute)),
            expr_to_constant<T>(
            expr->right_subexpr,
            master));
        else
        compiled_expr.push_instruction_1i(
            (is_extra ? gobio_opcodes::OPCODE_EXTRA_UASSIGN : gobio_opcodes::OPCODE_UASSIGN),
            reg.get_id(*(expr->left_subexpr->attribute)));
        break;
    }

    /* AND i OR ze względu na specyficzny sposób
       ewaluacji są obsługiwane także w głównej funkcji
           ELSE też tutaj pasuje, bo X :: Y zamieniamy na:
           X
           JMP E
           Y
          E:
    */
    case GRuleExpression::OPERATOR_AND:
    {
        int instruction_to_repair_ix = traversal_stack.back().address;

        // uwzględniamy optymalizację (patrz wyżej)
        if (instruction_to_repair_ix >= 0)
        compiled_expr.set_iarg(
            instruction_to_repair_ix,
            0,
            compiled_expr.first_available_address());
    }
    break;

    case GRuleExpression::OPERATOR_OR:
    case GRuleExpression::OPERATOR_ELSE:
    {
        int instruction_to_repair_ix = traversal_stack.back().address;
        compiled_expr.set_iarg(
        instruction_to_repair_ix,
        0,
        compiled_expr.first_available_address());
    }
    break;

    case GRuleExpression::OPERATOR_IMPLICATION:
    {
        int instruction_to_repair_ix = traversal_stack.back().address;
        compiled_expr.set_iarg(
        instruction_to_repair_ix,
        0,
        // dwie możliwości:
        (last_else_jmp_address >= 0 ?
         // z ELSE (w => x :: y)
         last_else_jmp_address :
         // bez ELSE (w => x)
         compiled_expr.first_available_address()));
    }
    break;

    case GRuleExpression::OPERATOR_EQUAL:
        if (is_constant(expr->right_subexpr))
        compiled_expr.push_instruction_1t(
            gobio_opcodes::OPCODE_EQUAL_CONST,
            expr_to_constant<T>(
            expr->right_subexpr,
            master));
        else
        compiled_expr.push_instruction_0(gobio_opcodes::OPCODE_EQUAL);
        break;

    case GRuleExpression::OPERATOR_UEQUAL:
        if (is_constant(expr->right_subexpr))
        compiled_expr.push_instruction_1t(
            gobio_opcodes::OPCODE_UEQUAL_CONST,
            expr_to_constant<T>(
            expr->right_subexpr,
            master));
        else
        compiled_expr.push_instruction_0(gobio_opcodes::OPCODE_UEQUAL);
        break;

    case GRuleExpression::OPERATOR_NOT_EQUAL:
        if (is_constant(expr->right_subexpr))
        compiled_expr.push_instruction_1t(
            gobio_opcodes::OPCODE_NOT_EQUAL_CONST,
            expr_to_constant<T>(
            expr->right_subexpr,
            master));
        else
        compiled_expr.push_instruction_0(gobio_opcodes::OPCODE_NOT_EQUAL);
        break;

    case GRuleExpression::OPERATOR_HOOKING:
        if (is_constant(expr->right_subexpr))
        compiled_expr.push_instruction_1t(
            gobio_opcodes::OPCODE_HOOKING_CONST,
            expr_to_constant<T>(
            expr->right_subexpr,
            master));
        else
        compiled_expr.push_instruction_0(gobio_opcodes::OPCODE_HOOKING);
        break;

    case GRuleExpression::OPERATOR_SEMANTICS_INTERSECTION:

        if (expr->left_subexpr->expr_type == GRuleExpression::OPERATOR_BI
           && expr->left_subexpr->expr_operator == GRuleExpression::OPERATOR_BINARG)
        compiled_expr.push_instruction_0(gobio_opcodes::OPCODE_TRI_SEMANTICS_INTERSECTION);
        else if (is_constant(expr->right_subexpr))
        compiled_expr.push_instruction_1t(
            gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION_CONST,
            expr_to_constant<T>(
            expr->right_subexpr,
            master));
        else
        compiled_expr.push_instruction_0(gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION);
        break;

    case GRuleExpression::OPERATOR_SUM:
        if (is_constant(expr->right_subexpr))
        compiled_expr.push_instruction_1t(
            gobio_opcodes::OPCODE_ADD_CONST,
            expr_to_constant<T>(
            expr->right_subexpr,
            master));
        else
        compiled_expr.push_instruction_0(gobio_opcodes::OPCODE_ADD);
        break;

    case GRuleExpression::OPERATOR_BINARG:
        // nic nie robimy!
        break;

    default:
        assert(0);

    }
    break;
    default:
    assert(0);
    }
}


#endif
