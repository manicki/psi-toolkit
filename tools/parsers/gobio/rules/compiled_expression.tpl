#ifndef COMPILED_EXPRESSION_TPL_HDR
#define COMPILED_EXPRESSION_TPL_HDR

#include "compiled_expression.hpp"
#include "opcodes.hpp"

#include <assert.h>

template<class T, class S, int N>
compiled_expression<T,S,N>::compiled_expression()
{
    instructions_.reserve(32);
}

template<class T, class S, int N>
void compiled_expression<T,S,N>::push_instruction_0(int op_code)
{
    instructions_.push_back(instruction());
    instructions_.back().op_code = op_code;
}

template<class T, class S, int N>
void compiled_expression<T,S,N>::push_instruction_1t(int op_code, T arg1)
{
    instructions_.push_back(instruction());
    instructions_.back().op_code = op_code;
    instructions_.back().instruction_arguments[0].t = arg1;
}

template<class T, class S, int N>
void compiled_expression<T,S,N>::push_instruction_1i(int op_code, int arg1)
{
    instructions_.push_back(instruction());
    instructions_.back().op_code = op_code;
    instructions_.back().instruction_arguments[0].i = arg1;
}

template<class T, class S, int N>
void compiled_expression<T,S,N>::push_instruction_1f(int op_code, S arg1)
{
    instructions_.push_back(instruction());
    instructions_.back().op_code = op_code;
    instructions_.back().instruction_arguments[0].f = arg1;
}

template<class T, class S, int N>
void compiled_expression<T,S,N>::push_instruction_2ii(int op_code, int arg1, int arg2)
{
    instructions_.push_back(instruction());
    instructions_.back().op_code = op_code;
    instructions_.back().instruction_arguments[0].i = arg1;
    instructions_.back().instruction_arguments[1].i = arg2;
}

template<class T, class S, int N>
void compiled_expression<T,S,N>::push_instruction_2it(int op_code, int arg1, T arg2)
{
    instructions_.push_back(instruction());
    instructions_.back().op_code = op_code;
    instructions_.back().instruction_arguments[0].i = arg1;
    instructions_.back().instruction_arguments[1].t = arg2;
}

template<class T, class S, int N>
void compiled_expression<T,S,N>::pop_instruction()
{
    instructions_.pop_back();
}

template<class T, class S, int N>
void compiled_expression<T,S,N>::pop_if_instruction(int opcode)
{
    if(!instructions_.empty())
    {
	if(instructions_.back().op_code == opcode)
	    instructions_.pop_back();
    }
}

template<class T, class S, int N>
void compiled_expression<T,S,N>::delete_instructions(int begin_ix, int end_ix)
{
    assert(begin_ix <= (signed int)instructions_.size());
    assert(end_ix   <= (signed int)instructions_.size());

    instructions_.erase(
	instructions_.begin() + begin_ix,
	instructions_.begin() + end_ix);
}

template<class T, class S, int N>
S compiled_expression<T,S,N>::get_score()
{
    for (int address = 0; address < (signed int)instructions_.size(); ++address) {
        if (instructions_[address].op_code == gobio_opcodes::OPCODE_SET_SCORE) {
            return instructions_[address].instruction_arguments[0].f;
        }
    }
    return 0;
}

template<class T, class S, int N>
void compiled_expression<T,S,N>::set_score(S score)
{
    for (int address = 0; address < (signed int)instructions_.size(); ++address) {
        if (instructions_[address].op_code == gobio_opcodes::OPCODE_SET_SCORE) {
            instructions_[address].instruction_arguments[0].f = score;
        }
    }
    push_instruction_1f(gobio_opcodes::OPCODE_SET_SCORE, score);
}

template<class T, class S, int N>
int compiled_expression<T,S,N>::get_opcode(int address) const
{
    assert(address >=0 && address < (signed int)instructions_.size());
    return instructions_[address].op_code;
}

template<class T, class S, int N>
T compiled_expression<T,S,N>::get_targ(int address, int arg_number) const
{
    assert(address >=0 && address < (signed int)instructions_.size());
    assert(arg_number >=0 && arg_number < N);
    return instructions_[address].instruction_arguments[arg_number].t;
}

template<class T, class S, int N>
int compiled_expression<T,S,N>::get_iarg(int address, int arg_number) const
{
    assert(address >=0 && address < (signed int)instructions_.size());
    assert(arg_number >=0 && arg_number < N);
    return instructions_[address].instruction_arguments[arg_number].i;
}


template<class T, class S, int N>
S compiled_expression<T,S,N>::get_farg(int address, int arg_number) const
{
    assert(address >=0 && address < (signed int)instructions_.size());
    assert(arg_number >=0 && arg_number < N);
    return instructions_[address].instruction_arguments[arg_number].f;
}

    
template<class T, class S, int N>
void compiled_expression<T,S,N>::set_iarg(int address, int arg_number, int arg_value)
{
    assert(address >=0 && address < (signed int)instructions_.size());
    assert(arg_number >=0 && arg_number < N);
    instructions_[address].instruction_arguments[arg_number].i = arg_value;
}

template<class T, class S, int N>
void compiled_expression<T,S,N>::set_farg(int address, int arg_number, S arg_value)
{
    assert(address >=0 && address < (signed int)instructions_.size());
    assert(arg_number >=0 && arg_number < N);
    instructions_[address].instruction_arguments[arg_number].f = arg_value;
}

template<class T, class S, int N>
void compiled_expression<T,S,N>::set_opcode(int address, int opc, bool clear_args)
{
    assert(address >=0 && address < (signed int)instructions_.size());
    instructions_[address].op_code = opc;

    if(clear_args)
    {
	for(int i = 0; i < N; ++i)
	    instructions_[address].instruction_arguments[i] 
		= typename instruction::instruction_argument();
    }
}


template<class T, class S, int N>
int compiled_expression<T,S,N>::first_available_address() const
{
    return instructions_.size();
}

template<class T, class S, int N>
void compiled_expression<T,S,N>::append(const compiled_expression& a)
{
    for(size_t i = 0; i < a.instructions_.size(); ++i)
	instructions_.push_back(a.instructions_[i]);
}

template<class T, class S, int N>
boost::shared_ptr<compiled_expression<T,S,N> > compiled_expression<T,S,N>::get_slice(
    int begin_ix, int end_ix)
{
    assert(begin_ix <= instructions_.size() && end_ix <= instructions_.size());

    boost::shared_ptr<compiled_expression<T,S,N> > r(
	new compiled_expression<T,S,N>());

    for(int ix = begin_ix; ix < end_ix; ++ix)
	r->instructions_.push_back(instructions_[ix]);

    return r;
}

template<class T, class S, int N>
unsigned int compiled_expression<T,S,N>::hash_fun() const
{
    int h = 0;
    for(size_t i = 0; i < instructions_.size(); ++i)
    {
	h ^= HASH_WRAPPER_FULL_HASH_TRAITS<int>().operator()(instructions_[i].op_code);

	h <<= 1;
	h ^= HASH_WRAPPER_FULL_HASH_TRAITS<int>().operator()(
	    instructions_[i].instruction_arguments[0].t);

	h <<= 1;
	h ^= HASH_WRAPPER_FULL_HASH_TRAITS<T>().operator()(
	    instructions_[i].instruction_arguments[0].i);
    }

    return h;
}

template<class T, class S, int N>
bool compiled_expression<T,S,N>::operator==(const compiled_expression& a) const
{
    if(instructions_.size() != a.instructions_.size())
	return false;

    for(size_t i = 0; i < instructions_.size(); ++i)
    {
	if(instructions_[i].op_code != a.instructions_[i].op_code)
	    return false;

	for(size_t j = 0; j < N; ++j)
	    if(instructions_[i].instruction_arguments[j].t
	       != a.instructions_[i].instruction_arguments[j].t
	       ||
	       instructions_[i].instruction_arguments[j].f
	       != a.instructions_[i].instruction_arguments[j].f
	       ||
	       instructions_[i].instruction_arguments[j].i
	       != a.instructions_[i].instruction_arguments[j].i)
		return false;
    }

    return true;
}

template<class T, class S, int N>
bool compiled_expression<T,S,N>::operator!=(const compiled_expression& a) const
{
    return !operator==(a);
}

#endif
