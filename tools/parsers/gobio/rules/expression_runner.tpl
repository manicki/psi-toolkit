#ifndef EXPRESSION_RUNNER_TPL_HDR
#define EXPRESSION_RUNNER_TPL_HDR

#include "atom_converter.hpp"
#include "av_matrix.hpp"
#include "compiled_expression.tpl"
#include "expression_runner.hpp"
#include "opcodes.hpp"
#include "registrar.tpl"

#include <iostream>
#include <stdexcept>

template<class C, class T, class S, class M, class X, bool checking>
expression_runner<C,T,S,M,X,checking>::expression_runner()
{
}

template<class C, class T, class S, class M, class X, bool checking>
bool expression_runner<C,T,S,M,X,checking>::check_hook_(int h, int q, int r, int& v)
{
    if ((q & h) && (r & h))
    {
        v = -1L;
        return false;
    }

    if ((q & h) || (r & h))
    {
        v |= h;
    }

    return true;
}

template<class C, class T, class S, class M, class X, bool checking>
T expression_runner<C,T,S,M,X,checking>::hooking(T a, T b, M& master)
{
    if (!master.is_int(a) || !master.is_int(b))
        return master.false_value();

    int r = master.to_int(a);
    int q = master.to_int(b);

    int v = 0;

    check_hook_(1, r, q, v)
        && check_hook_(2, r, q, v)
        && check_hook_(4, r, q, v)
        && check_hook_(8, r, q, v)
        && check_hook_(16, r, q, v)
        && check_hook_(32, r, q, v)
        && check_hook_(64, r, q, v)
        && check_hook_(128, r, q, v);

    if (v == -1L)
        return master.false_value();

    return master.from_int(v);
}

template<class C, class T, class S, class M, class X, bool checking>
T expression_runner<C,T,S,M,X,checking>::add_(T a, T b, M& master)
{
    int r = AtomConverter::toInt(master.false_value());

    if (
        master.is_int(a) &&
        master.is_int(b) &&
        master.to_int(a) >= 0 &&
        master.to_int(b) >= 0
    ) {
        r = master.to_int(a) + master.to_int(b);

        // if (r > master.int_limit()) return AtomConverter::toAtom<T>(master.int_limit());
    }

    return AtomConverter::toAtom<T>(r);
}

template<class C, class T, class S, class M, class X, bool checking>
void expression_runner<C,T,S,M,X,checking>::clear()
{
    stack_ = std::stack<T>();
}

template<class C, class T, class S, class M, class X, bool checking>
bool expression_runner<C,T,S,M,X,checking>::run(
    compiled_expression<T, S, 2>& expr,
    runner_frame& frame,
    M& master,
    X& semantics_machine)
{
    // puste wyrazenie nie musi miec instrukcji STOP
    if (expr.first_available_address() == 0)
        return false;

    assert(expr.first_available_address() > 0);
    assert(expr.get_opcode(expr.first_available_address()-1) == gobio_opcodes::OPCODE_STOP);

    S bonus = 0;

    int i = 0;
    while (1)
    {
        if (checking)
        {
            if (i >= expr.first_available_address())
            throw std::runtime_error("wrong instruction index");
        }

        switch(expr.get_opcode(i))
        {
        case gobio_opcodes::OPCODE_ADD:
        {
            check_stack_();
            T q = stack_.top();
            stack_.pop();

            check_stack_();
            stack_.top() = add_(q, stack_.top(), master);
        }
        break;

        case gobio_opcodes::OPCODE_ADD_CONST:
            check_stack_();
            stack_.top() = add_(stack_.top(), expr.get_targ(i, 0), master);
            break;

        case gobio_opcodes::OPCODE_ALWAYS_FALSE:
            check_stack_();
            stack_.top() = master.false_value();
            break;

        case gobio_opcodes::OPCODE_ALWAYS_TRUE:
            check_stack_();
            stack_.top() = master.any_value();
            break;

        case gobio_opcodes::OPCODE_ASSIGN:
            check_stack_();
            frame.lhs->set_attr(
            expr.get_iarg(i, 0),
            stack_.top(),
            master.false_value());
            break;

        case gobio_opcodes::OPCODE_ASSIGN_CONST:
            stack_.push(
            frame.lhs->set_attr(
                expr.get_iarg(i, 0),
                expr.get_targ(i, 1),
                master.false_value()));
            break;

        case gobio_opcodes::OPCODE_EQUAL:
        {
            check_stack_();
            T t = stack_.top();
            stack_.pop();

            check_stack_();
            stack_.top() =
            master.from_bool(t == stack_.top());
        }
        break;

        case gobio_opcodes::OPCODE_EQUAL_CONST:
            check_stack_();
            stack_.top() =
            master.from_bool((stack_.top() == expr.get_targ(i, 0)));
            break;

        case gobio_opcodes::OPCODE_EXTRA_ASSIGN:
            check_stack_();
            assert(frame.extra_lhs);
            frame.extra_lhs->set_attr(
            expr.get_iarg(i, 0),
            stack_.top(),
            master.false_value());
            break;

        case gobio_opcodes::OPCODE_EXTRA_ASSIGN_CONST:
            assert(frame.extra_lhs);
            stack_.push(
            frame.extra_lhs->set_attr(
                expr.get_iarg(i, 0),
                expr.get_targ(i, 1),
                master.false_value()));
            break;

        case gobio_opcodes::OPCODE_EXTRA_UASSIGN:
        {
            assert(frame.extra_lhs);
            T v = frame.extra_lhs->get_attr(expr.get_iarg(i, 0), master.false_value());

            check_stack_();

            if (v != stack_.top())
            {
            if (master.is_any(v))
                stack_.top() = frame.extra_lhs->set_attr(
                expr.get_iarg(i, 0),
                stack_.top(),
                master.false_value());
            else if (master.is_any(stack_.top()))
                stack_.top() = v;
            else
                stack_.top() = master.false_value();
            }
            // else: wartosc zostaje na stosie
        }
        break;


        case gobio_opcodes::OPCODE_EXTRA_UASSIGN_CONST:
        {
            assert(frame.extra_lhs);
            T v = frame.extra_lhs->get_attr(expr.get_iarg(i, 0), master.false_value());
            T a = expr.get_targ(i, 1);

            if (v != a)
            {
            if (master.is_any(v))
                stack_.push(
                frame.extra_lhs->set_attr(
                    expr.get_iarg(i, 0),
                    a,
                    master.false_value()));
            else if (master.is_any(a))
                stack_.push(v);
            else
                stack_.push(master.false_value());
            }
            else
            stack_.push(a);
        }
        break;

        case gobio_opcodes::OPCODE_HOOKING:
        {
            check_stack_();
            T q = stack_.top();
            stack_.pop();

            check_stack_();
            stack_.top() = hooking(q, stack_.top(), master);
        }
        break;

        case gobio_opcodes::OPCODE_HOOKING_CONST:
            check_stack_();
            stack_.top() = hooking(stack_.top(), expr.get_targ(i, 0), master);
            break;

        case gobio_opcodes::OPCODE_IFJUMP:
            check_stack_();
            if (master.is_true(stack_.top()))
            {
            i = expr.get_iarg(i, 0);
            continue;
            }
            break;

        case gobio_opcodes::OPCODE_IFNJUMP:
            check_stack_();
            if (master.is_false(stack_.top()))
            {
            i = expr.get_iarg(i, 0);
            continue;
            }
            break;

        case gobio_opcodes::OPCODE_JUMP:
            i = expr.get_iarg(i, 0);
            continue;

        case gobio_opcodes::OPCODE_NOP:
            break;

        case gobio_opcodes::OPCODE_NOT:
            check_stack_();
            stack_.top() =
            master.from_bool(!master.is_true(stack_.top()));
            break;

        case gobio_opcodes::OPCODE_NOT_EQUAL:
        {
            check_stack_();
            T t = stack_.top();
            stack_.pop();

            check_stack_();
            stack_.top() =
            master.from_bool(t != stack_.top());
        }
        break;

        case gobio_opcodes::OPCODE_NOT_EQUAL_CONST:
            check_stack_();
            stack_.top() =
            master.from_bool((stack_.top() != expr.get_targ(i, 0)));
            break;

        case gobio_opcodes::OPCODE_POP:
            check_stack_();
            stack_.pop();
            break;

        case gobio_opcodes::OPCODE_PRINT:
            check_stack_();

            if (master.is_string(stack_.top()))
            std::cerr << master.to_string(stack_.top()) << std::endl;
            else if (master.is_int(stack_.top()))
            std::cerr << master.to_int(stack_.top()) << std::endl;
            else
            std::cerr << "???\n";

            stack_.top() = master.any_value();
            break;

        case gobio_opcodes::OPCODE_PUSH:
            stack_.push(expr.get_targ(i, 0));
            break;

        case gobio_opcodes::OPCODE_PUSH_EXTRA_SUBVAR:
            if (checking)
            {
            if (expr.get_iarg(i, 1) >= (signed int)frame.extra_rhs.size())
                throw std::runtime_error("PUSH subvar overflow");
            }

            stack_.push(
            frame.extra_rhs[expr.get_iarg(i, 1)]->get_attr(
                expr.get_iarg(i, 0),
                master.false_value()));
            break;

        case gobio_opcodes::OPCODE_PUSH_EXTRA_VAR:
            assert(frame.extra_lhs);
            stack_.push(
            frame.extra_lhs->get_attr(expr.get_iarg(i, 0), master.false_value()));
            break;

        case gobio_opcodes::OPCODE_PUSH_FALSE:
            stack_.push(master.false_value());
            break;

        case gobio_opcodes::OPCODE_PUSH_SUBVAR:
            if (checking)
            {
            if (expr.get_iarg(i, 1) >= (signed int)frame.rhs.size())
                throw std::runtime_error("PUSH subvar overflow");
            }

            stack_.push(
            frame.rhs[expr.get_iarg(i, 1)]->get_attr(
                expr.get_iarg(i, 0),
                master.false_value()));
            break;

        case gobio_opcodes::OPCODE_PUSH_TRUE:
            stack_.push(master.any_value());
            break;

        case gobio_opcodes::OPCODE_PUSH_VAR:
            stack_.push(
            frame.lhs->get_attr(expr.get_iarg(i, 0), master.false_value()));
            break;


        case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION:
        {
            check_stack_();
            T q = stack_.top();
            stack_.pop();

            check_stack_();
            S intersection_bonus;
            stack_.top() = semantics_machine.intersect(stack_.top(), q, intersection_bonus);
            bonus += intersection_bonus;
        }
        break;

        case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION_CONST:
        {
            check_stack_();
            S intersection_bonus;
            stack_.top() = semantics_machine.intersect(
            stack_.top(), expr.get_targ(i, 0), intersection_bonus);
            bonus += intersection_bonus;
        }
        break;

        case gobio_opcodes::OPCODE_SETTOP:
            check_stack_();
            stack_.top() = expr.get_targ(i, 0);
            break;

        case gobio_opcodes::OPCODE_SETTOP_EXTRA_SUBVAR:
            check_stack_();

            if (checking)
            {
            if (expr.get_iarg(i, 1) >= (signed int)frame.extra_rhs.size())
                throw std::runtime_error("SETTOP subvar overflow");
            }

            stack_.top() =
            frame.extra_rhs[expr.get_iarg(i, 1)]->get_attr(
                expr.get_iarg(i, 0),
                master.false_value());
            break;

        case gobio_opcodes::OPCODE_SETTOP_EXTRA_VAR:
            check_stack_();
            assert(frame.extra_lhs);

            stack_.top() =
            frame.extra_lhs->get_attr(expr.get_iarg(i, 0), master.false_value());
            break;

        case gobio_opcodes::OPCODE_SETTOP_SUBVAR:
            check_stack_();

            if (checking)
            {
            if (expr.get_iarg(i, 1) >= (signed int)frame.rhs.size())
                throw std::runtime_error("SETTOP subvar overflow");
            }

            stack_.top() =
            frame.rhs[expr.get_iarg(i, 1)]->get_attr(
                expr.get_iarg(i, 0),
                master.false_value());
            break;

        case gobio_opcodes::OPCODE_SETTOP_VAR:
            check_stack_();

            stack_.top() =
            frame.lhs->get_attr(expr.get_iarg(i, 0), master.false_value());
            break;


        case gobio_opcodes::OPCODE_SET_SCORE:
            frame.score = expr.get_farg(i, 0);

            stack_.push(master.any_value());
            break;

        case gobio_opcodes::OPCODE_STOP:
            // nie mozna uzyc po prostu break-a, bo jestesmy wewnatrz switch-a
            goto loop_exit;

        case gobio_opcodes::OPCODE_TREE_CHOICE:
            frame.chosen_tree_ix = expr.get_iarg(i, 0);

            stack_.push(master.any_value());
            break;

        case gobio_opcodes::OPCODE_TRI_SEMANTICS_INTERSECTION:
        {
            check_stack_();
            T w = stack_.top();
            stack_.pop();

            check_stack_();
            T q = stack_.top();
            stack_.pop();

            check_stack_();
            S intersection_bonus;
            stack_.top() = semantics_machine.tri_intersect(stack_.top(), q, w, intersection_bonus);
            bonus += intersection_bonus;
        }
        break;

        case gobio_opcodes::OPCODE_UASSIGN:
        {
            T v = frame.lhs->get_attr(expr.get_iarg(i, 0), master.false_value());

            check_stack_();

            if (v != stack_.top())
            {
            if (master.is_any(v))
                stack_.top() = frame.lhs->set_attr(
                expr.get_iarg(i, 0),
                stack_.top(),
                master.false_value());
            else if (master.is_any(stack_.top()))
                stack_.top() = v;
            else
                stack_.top() = master.false_value();
            }
            // else: wartosc zostaje na stosie
        }
        break;


        case gobio_opcodes::OPCODE_UASSIGN_CONST:
        {
            T v = frame.lhs->get_attr(expr.get_iarg(i, 0), master.false_value());
            T a = expr.get_targ(i, 1);

            if (v != a)
            {
            if (master.is_any(v))
                stack_.push(
                frame.lhs->set_attr(
                    expr.get_iarg(i, 0),
                    a,
                    master.false_value()));
            else if (master.is_any(a))
                stack_.push(v);
            else
                stack_.push(master.false_value());
            }
            else
            stack_.push(a);
        }
        break;

        case gobio_opcodes::OPCODE_UEQUAL:
        {
            check_stack_();
            T q = stack_.top();
            stack_.pop();

            check_stack_();

            if (q == stack_.top() || master.is_any(stack_.top()))
            stack_.top() = q;
            else if (master.is_any(q))
            stack_.top() = stack_.top();
            else
            stack_.top() = master.false_value();
        }
        break;


        case gobio_opcodes::OPCODE_UEQUAL_CONST:
        {
            T a = expr.get_targ(i, 0);
            check_stack_();

            if (a == stack_.top() || master.is_any(stack_.top()))
            stack_.top() = a;
            else if (master.is_any(a))
            stack_.top() = stack_.top();
            else
            stack_.top() = master.false_value();
        }
        break;
        default:
            assert(0);

        }

        // w przypadku instrukcji JUMP, IFJUMP, IFNJUMP, STOP nie dochodzimy
        // do tego miejsca
        ++i;
    }
loop_exit:

    frame.score += bonus;

    check_stack_();
    return master.is_true(stack_.top());
}

template<class C, class T, class S, class M, class X, bool checking>
void expression_runner<C,T,S,M,X,checking>::check_stack_()
{
    if (checking)
    {
        if (stack_.empty())
            throw std::runtime_error("empty stack");
    }
}


template<class C, class T, class S, class M, class X, bool checking>
S expression_runner<C,T,S,M,X,checking>::get_score(compiled_expression<T, S, 2>& expr) {
    assert(expr.first_available_address() > 0);
    assert(expr.get_opcode(expr.first_available_address()-1) == gobio_opcodes::OPCODE_STOP);
    S score;
    int setscores = 0;
    int i = 0;
    while (true) {
        if (expr.get_opcode(i) == gobio_opcodes::OPCODE_STOP) {
            break;
        }
        if (expr.get_opcode(i) == gobio_opcodes::OPCODE_SET_SCORE) {
            ++setscores;
            score = expr.get_farg(i, 0);
        }
        ++i;
    }
    assert(setscores <= 1);
    return score;
}



#endif
