#ifndef COMPILED_EXPRESSION_HPP_HDR
#define COMPILED_EXPRESSION_HPP_HDR

#include <vector>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <boost/shared_ptr.hpp>

#include "hash_wrapper.hpp"

template<class T, class S, int N>
class compiled_expression
{
public:
    compiled_expression();

    void push_instruction_0(int op_code);

    void push_instruction_1t(int op_code, T arg1);
    void push_instruction_1i(int op_code, int arg1);
    void push_instruction_1f(int op_code, S arg1);

    void push_instruction_2ii(int op_code, int arg1, int arg2);
    void push_instruction_2it(int op_code, int arg1, T arg2);

    void pop_instruction();
    void pop_if_instruction(int op_code);

    void delete_instructions(int begin_ix, int end_ix);

    S get_score();
    void set_score(S score);

    int get_opcode(int address) const;
    T   get_targ(int address, int arg_number) const;
    int get_iarg(int address, int arg_number) const;
    S get_farg(int address, int arg_number) const;

    void set_iarg(int address, int arg_number, int arg_value);
    void set_farg(int address, int arg_number, S arg_value);

    void set_opcode(int address, int opc, bool clear_args = false);

    int first_available_address() const;

    void append(const compiled_expression& a);

    boost::shared_ptr<compiled_expression> get_slice(int begin_ix, int end_ix);

    unsigned int hash_fun() const;

    bool operator==(const compiled_expression& a) const;
    bool operator!=(const compiled_expression& a) const;

private:
    struct instruction
    {
    int op_code;

    struct instruction_argument
    {
        T t;
        int i;
        S f;
    };

    instruction_argument instruction_arguments[N];

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);
    };

    std::vector<instruction> instructions_;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*file_version*/)
    {
        ar & instructions_;
    }
};

template<class T, class S, int N>
struct compiled_expression_hash_fun
{
    HASH_WRAPPER_EXTRA_STUFF

    unsigned int operator()(const compiled_expression<T, S, N>& k) const
    {
        return k.hash_fun();
    }

#ifdef __VS__
    bool operator()(const compiled_expression<T, S, N>& a,
            const compiled_expression<T, S, N>& b) const
    {
        return a != b;
    }
#endif

};

template<class T, class S, int N>
struct int_compiled_expression_hash_fun
{
    HASH_WRAPPER_EXTRA_STUFF

    unsigned int operator()(const std::pair<int, compiled_expression<T, S, N> >& k) const
    {
        return HASH_WRAPPER_FULL_HASH_TRAITS<int>().operator()(k.first << 11)
            ^  k.second.hash_fun();
    }

#ifdef __VS__
    bool operator()(const std::pair<int, compiled_expression<T, S, N> >& a,
            const std::pair<int, compiled_expression<T, S, N> >& b) const
    {
        return a != b;
    }
#endif

};


#endif
