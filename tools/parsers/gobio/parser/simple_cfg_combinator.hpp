#ifndef SIMPLE_CFG_COMBINATOR_HPP_HDR
#define SIMPLE_CFG_COMBINATOR_HPP_HDR

#include <vector>
#include <list>
#include <limits>

// "realizacja" typu R
template<typename T>
class simple_cfg_rule
{
private:
    T lhs_;
    double score_;
    int rule_no_;

    static int last_rule_no_;

public:
    simple_cfg_rule(T lhs, double score=0.0):lhs_(lhs), score_(score), rule_no_(0) {}

    T category()   const { return lhs_; }
    double score() const { return score_; }
    int rule_no()  const { return rule_no_; }
};


template<typename T, typename R>
class simple_cfg_combinator
{
public:
    void add_unary_rule (T rhs,                     R rule);
    void add_binary_rule(T first_rhs, T second_rhs, R rule);

    std::vector<R> combine_unary(T symbol);
    std::vector<R> combine_binary(T first_symbol, T second_symbol);

    // znajduje możliwych lewych braci (aktualnie nieużywane)
    std::vector<T> find_left_sibling(R parent_rule, T second_symbol);

    // znajduje możliwych prawych braci (aktualnie nieużywane)
    std::vector<T> find_right_sibling(R parent_rule, T first_symbol);

    // znajduje możliwych rodziców jedynaków
    std::vector<R> find_only_child_parent(T symbol);

    // znajduje możliwych lewych braci z rodzicami
    std::vector<std::pair<R, T> > find_left_sibling_with_parent(T second_symbol);

    // znajduje możliwych prawych braci z rodzicami
    std::vector<std::pair<R, T> > find_right_sibling_with_parent(T first_symbol);

    // znajduje reguły binarne, których głową jest dany węzeł
    std::vector< std::pair< R, std::pair<T, T> > > split(T parent_symbol);

    // znajduje reguły unarne, których głową jest dany węzeł
    std::vector< std::pair< R, T > > find_only_children(T parent_symbol);

    // dodaje wierzchołek do listy korzeni
    void set_root(T state);

    // sprawdza, czy jest korzeniem
    bool is_root(T state);

    // sprawdza, czy jest symbolem terminalnym
    bool is_terminal(T state) {
        return ((split(state).size()==0)&&(find_only_children(state).size()==0));
    }

    // przeniosłem do sx.hpp
    // dodaje wierzchołek do listy użytych (aby zapobiegać pętlom reguł unarnych)
//  void mark_as_used(T state);

    // przeniosłem do sx.hpp
    // sprawdza, czy wierzchołek został użyty (aby zapobiegać pętlom reguł unarnych)
//  bool is_used(T state);

    // przeniosłem do sx.hpp
    // czyści listę zużytych wierzchołków
//  void clear_used();

    bool is_useful(T /*symbol*/) const { return true; }

    void clear_parser_caches() {}

    size_t get_nb_limits() const { return 1; }
    int get_limit_threshold(size_t /*limit_ix*/) const
    { return std::numeric_limits<int>::max(); }
    double get_limit_min_score(size_t /*limit_ix*/) const
    { return -std::numeric_limits<double>::max(); }

    int get_limit_edges(int len) const
    { return -1L; }

private:
    struct unary_rule_entry
    {
    T rhs;
    R rule;

    unary_rule_entry(T a_rhs, R a_rule):
        rhs(a_rhs), rule(a_rule) {}
    };

    struct binary_rule_entry
    {
    T  first_rhs;
    T second_rhs;
    R rule;

    binary_rule_entry(T a_first_rhs, T a_second_rhs, R a_rule):
        first_rhs(a_first_rhs), second_rhs(a_second_rhs),
        rule(a_rule) {}
    };

    std::list<unary_rule_entry > unary_rules_;
    std::list<binary_rule_entry> binary_rules_;

    // lista korzeni
    std::list<T> roots_;

    // przeniosłem do sx.hpp
    // lista wierzchołków użytych przy znajdywaniu jedynaków
//  std::list<T> used_;
};

#endif
