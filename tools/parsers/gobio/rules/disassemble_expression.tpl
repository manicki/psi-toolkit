#ifndef DISASSEMBLE_EXPRESSION_TPL_HDR
#define DISASSEMBLE_EXPRESSION_TPL_HDR

#include "disassemble_expression.hpp"

#include <iomanip>
#include <string>

#include "registrar.tpl"

template<class T, class M>
void print_value(
    M& master,
    T val,
    ostream& s)
{
    if (master.is_string(val))
	s << master.to_string(val);
    else if (master.is_int(val))
	s << master.to_int(val);
    else
	s << "???";
}


template<class T, class S, int N, class M>
void disassemble_expression(
    const compiled_expression<T,S,N>& expr,
    const registrar<std::string>& attribute_registrar,
    const registrar<std::string>& extra_attribute_registrar,
    const M& master,
    std::ostream& s)
{
    for (int i=0; i < expr.first_available_address(); ++i)
    {
	s << std::setw(4) << std::setfill('0') << i;

	s << ".  ";

	switch(expr.get_opcode(i))
	{
	case gobio_opcodes::OPCODE_ADD:
	    s << "ADD";
	    break;

	case gobio_opcodes::OPCODE_ADD_CONST:
	    s << "ADD ";
	    print_value(master, expr.get_targ(i, 0), s);
	    break;

	case gobio_opcodes::OPCODE_ALWAYS_FALSE:
	    s << "FALSE";
	    break;

	case gobio_opcodes::OPCODE_ALWAYS_TRUE:
	    s << "TRUE";
	    break;

	case gobio_opcodes::OPCODE_ASSIGN:
	    s << "ASSIGN "
	      << attribute_registrar.get_obj(expr.get_iarg(i, 0));
	    break;

	case gobio_opcodes::OPCODE_ASSIGN_CONST:
	    s << "ASSIGN "
	      << attribute_registrar.get_obj(expr.get_iarg(i, 0))
	      << ", ";
	    print_value(master, expr.get_targ(i, 1), s);
	    break;

	case gobio_opcodes::OPCODE_EQUAL:
	    s << "EQUAL";
	    break;

	case gobio_opcodes::OPCODE_EXTRA_ASSIGN:
	    s << "ASSIGN EXTRA "
	      << extra_attribute_registrar.get_obj(expr.get_iarg(i, 0));
	    break;

	case gobio_opcodes::OPCODE_EXTRA_ASSIGN_CONST:
	    s << "ASSIGN EXTRA "
	      << extra_attribute_registrar.get_obj(expr.get_iarg(i, 0))
	      << ", ";
	    print_value(master, expr.get_targ(i, 1), s);
	    break;

	case gobio_opcodes::OPCODE_EXTRA_UASSIGN:
	    s << "UASSIGN EXTRA "
	      << extra_attribute_registrar.get_obj(expr.get_iarg(i, 0));
	    break;

	case gobio_opcodes::OPCODE_EXTRA_UASSIGN_CONST:
	    s << "UASSIGN EXTRA "
	      << extra_attribute_registrar.get_obj(expr.get_iarg(i, 0))
	      << ", ";
	    print_value(master, expr.get_targ(i, 1), s);
	    break;

	case gobio_opcodes::OPCODE_EQUAL_CONST:
	    s << "EQUAL ";
	    print_value(master, expr.get_targ(i, 0), s);
	    break;

	case gobio_opcodes::OPCODE_HOOKING:
	    s << "HOOKING";
	    break;

	case gobio_opcodes::OPCODE_HOOKING_CONST:
	    s << "HOOKING ";
	    print_value(master, expr.get_targ(i, 0), s);
	    break;

	case gobio_opcodes::OPCODE_IFJUMP:
	    s << "IFJUMP "
	      << expr.get_iarg(i, 0);
	    break;

	case gobio_opcodes::OPCODE_IFNJUMP:
	    s << "IFNJUMP "
	      << expr.get_iarg(i, 0);
	    break;

	case gobio_opcodes::OPCODE_JUMP:
	    s << "JUMP "
	      << expr.get_iarg(i, 0);
	    break;

	case gobio_opcodes::OPCODE_NOP:
	    s << "NOP";
	    break;

	case gobio_opcodes::OPCODE_NOT:
	    s << "NOT";
	    break;

	case gobio_opcodes::OPCODE_NOT_EQUAL:
	    s << "NOT EQUAL";
	    break;

	case gobio_opcodes::OPCODE_NOT_EQUAL_CONST:
	    s << "NOT EQUAL ";
	    print_value(master, expr.get_targ(i, 0), s);
	    break;

	case gobio_opcodes::OPCODE_POP:
	    s << "POP";
	    break;

	case gobio_opcodes::OPCODE_PRINT:
	    s << "PRINT";
	    break;

	case gobio_opcodes::OPCODE_PUSH:
	    s << "PUSH " ;
	    print_value(master, expr.get_targ(i, 0), s);
	    break;

	case gobio_opcodes::OPCODE_PUSH_FALSE:
	    s << "PUSH FALSE";
	    break;

	case gobio_opcodes::OPCODE_PUSH_EXTRA_SUBVAR:
	    s << "PUSH EXTRA SUBVAR "
	      << extra_attribute_registrar.get_obj(expr.get_iarg(i, 0))
	      << '['
	      << expr.get_iarg(i, 1)
	      << ']';
	    break;

	case gobio_opcodes::OPCODE_PUSH_EXTRA_VAR:
	    s << "PUSH EXTRA VAR " << extra_attribute_registrar.get_obj(expr.get_iarg(i, 0));
	    break;

	case gobio_opcodes::OPCODE_PUSH_SUBVAR:
	    s << "PUSH SUBVAR "
	      << attribute_registrar.get_obj(expr.get_iarg(i, 0))
	      << '['
	      << expr.get_iarg(i, 1)
	      << ']';
	    break;

	case gobio_opcodes::OPCODE_PUSH_TRUE:
	    s << "PUSH TRUE";
	    break;

	case gobio_opcodes::OPCODE_PUSH_VAR:
	    s << "PUSH VAR " << attribute_registrar.get_obj(expr.get_iarg(i, 0));
	    break;

	case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION:
	    s << "INTERSECTION";
	    break;

	case gobio_opcodes::OPCODE_SEMANTICS_INTERSECTION_CONST:
	    s << "INTERSECTION ";
	    print_value(master, expr.get_targ(i, 0), s);
	    break;

	case gobio_opcodes::OPCODE_SET_SCORE:
	    s << "SET SCORE " << expr.get_farg(i, 0);
	    break;

	case gobio_opcodes::OPCODE_SETTOP:
	    s << "SETTOP " ;
	    print_value(master, expr.get_targ(i, 0), s);
	    break;

	case gobio_opcodes::OPCODE_SETTOP_EXTRA_SUBVAR:
	    s << "SETTOP EXTRA SUBVAR "
	      << extra_attribute_registrar.get_obj(expr.get_iarg(i, 0))
	      << '['
	      << expr.get_iarg(i, 1)
	      << ']';
	    break;

	case gobio_opcodes::OPCODE_SETTOP_EXTRA_VAR:
	    s << "SETTOP EXTRA VAR " << extra_attribute_registrar.get_obj(expr.get_iarg(i, 0));
	    break;

	case gobio_opcodes::OPCODE_SETTOP_SUBVAR:
	    s << "SETTOP SUBVAR "
	      << attribute_registrar.get_obj(expr.get_iarg(i, 0))
	      << '['
	      << expr.get_iarg(i, 1)
	      << ']';
	    break;

	case gobio_opcodes::OPCODE_SETTOP_VAR:
	    s << "SETTOP VAR " << attribute_registrar.get_obj(expr.get_iarg(i, 0));
	    break;

	case gobio_opcodes::OPCODE_STOP:
	    s << "STOP";
	    break;

	case gobio_opcodes::OPCODE_TREE_CHOICE:
	    s << "TREE CHOICE " << expr.get_iarg(i, 0);
	    break;

	case gobio_opcodes::OPCODE_TRI_SEMANTICS_INTERSECTION:
	    s << "TRI INTERSECTION";
	    break;

	case gobio_opcodes::OPCODE_UASSIGN:
	    s << "UASSIGN "
	      << attribute_registrar.get_obj(expr.get_iarg(i, 0));
	    break;

	case gobio_opcodes::OPCODE_UASSIGN_CONST:
	    s << "UASSIGN "
	      << attribute_registrar.get_obj(expr.get_iarg(i, 0))
	      << ", ";
	    print_value(master, expr.get_targ(i, 1), s);
	    break;

	case gobio_opcodes::OPCODE_UEQUAL:
	    s << "UEQUAL";
	    break;

	case gobio_opcodes::OPCODE_UEQUAL_CONST:
	    s << "UEQUAL ";
	    print_value(master, expr.get_targ(i, 0), s);
	    break;

	default:
	    assert(0);
	}

	s << "\n";
    }
}


#endif
