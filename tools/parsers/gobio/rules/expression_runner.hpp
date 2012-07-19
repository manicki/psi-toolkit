#ifndef EXPRESSION_RUNNER_HPP_HDR
#define EXPRESSION_RUNNER_HPP_HDR

#include <stack>
#include <vector>

#include "compiled_expression.hpp"
#include "av_matrix.hpp"
#include "registrar.hpp"

template<class C, class T, class S, class M, class X, bool checking>
class expression_runner
{
public:
    struct runner_frame
    {
	S score;
	int chosen_tree_ix;
	av_matrix<C, T>* lhs;
	typename bare_av_matrix<T>::type* extra_lhs;
	std::vector<const av_matrix<C, T>*> rhs;
	std::vector<const typename bare_av_matrix<T>::type*> extra_rhs;

	runner_frame():score(0),chosen_tree_ix(0),lhs(0), rhs(0) { }
    };

    expression_runner();

    void clear();

    bool run(
	compiled_expression<T,S,2>& expr,
	runner_frame& frame,
	M& master,
	X& semantics_machine);
	
	S get_score(compiled_expression<T,S,2>& expr);

private:
    std::stack<T> stack_;

    void check_stack_();
    bool check_hook_(int h, int q, int r, int& v);
    
    T add_(T a, T b, M& master);
    T hooking(T a, T b, M& master);
};

template<class T, class M, class S>
class semantics_stub
{
private:
    M& master_;

public:
    semantics_stub(M& master):master_(master) {}

    T intersect(T, T, S& bonus) { bonus = 0; return master_.any_value();}
    T tri_intersect(T, T, T, S& bonus) { bonus = 0; return master_.any_value();}
};

#endif
