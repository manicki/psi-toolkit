#ifndef TGBG_COMBINATOR_HPP_HDR
#define TGBG_COMBINATOR_HPP_HDR

#include <iostream>
#include <list>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// #include <boost/archive/binary_oarchive.hpp>
// #include <boost/archive/binary_iarchive.hpp>
// #include <boost/serialization/split_member.hpp>
// #include <boost/serialization/shared_ptr.hpp>
// #include <boost/serialization/vector.hpp>
// #include <boost/serialization/map.hpp>
// #include <boost/shared_ptr.hpp>

#include "av_matrix.hpp"
#include "compiled_expression.hpp"
#include "expression_runner.hpp"
#include "grules.h"
#include "hash_wrapper.hpp"
#include "registrar.tpl"
// #include "serialize_expression.hpp"
#include "tree_specification.hpp"

#ifdef _T5_WITH_EXTSCORES
#include "rule_score_reader.hpp"
#endif


//#include "tgbg_to_simple_cfg_projector.tpl"
template<typename T, typename S, typename M, typename X, typename E>
class tgbg_to_simple_cfg_projector;

//#include "tgbg_to_tab_int_projector.tpl"
template<typename T, typename S, typename M, typename X, typename E>
class tgbg_to_tab_int_projector;

//#include "tgbg_attribute_projector.tpl"
template<typename T, typename S, typename M, typename X, typename E>
class tgbg_attribute_projector;


HASH_WRAPPER_HASH_INTRO
    template<>
    struct HASH_WRAPPER_HASH_TRAITS<std::pair<std::pair<int, int>, int> >
    {
        HASH_WRAPPER_EXTRA_STUFF

        size_t operator()(const std::pair<std::pair<int, int>, int>& k) const
        {
            return HASH_WRAPPER_HASH_TRAITS<int>().operator()(k.first.first)
                & HASH_WRAPPER_HASH_TRAITS<int>().operator()(k.first.second)
                & HASH_WRAPPER_HASH_TRAITS<int>().operator()(k.second);
        }

#ifdef __VS__
        bool operator()(const std::pair<std::pair<int, int>, int>& a,
                const std::pair<std::pair<int, int>, int>& b) const
        {
            return a != b;
        }
#endif
    };
HASH_WRAPPER_HASH_OUTRO

struct empty_E
{
    operator bool() { return true; }
};

// E - dodatkowe informacje (odpowiednik)
template<class T, class S, class M, class X, class E = empty_E>
class tgbg_combinator
{
public:
    typedef T atom_type;
    typedef E equivalent_type;

    tgbg_combinator(M & master, bool with_decomposition=false);
    ~tgbg_combinator();

    void add_rules(std::istream& rules_stream);
    void add_rules(const std::string& filename);

#ifdef _T5_WITH_EXTSCORES
    void read_extscores(rule_score_reader<S>* rs_reader);
#endif


    void compile_all_rules();
    void index_all_rules();
    void clear_indexes();
    void binarize_all_rules();
    void decompose_all_rules();

    void purge_rules();

    typedef int BaseCategory;
    // typedef std::string BaseCategory;

    typedef av_matrix<BaseCategory, atom_type> entry_type;

    typedef typename bare_av_matrix<atom_type>::type variant_type;

    struct rule_holder
    {
    boost::shared_ptr<GRule> g_rule;

    boost::shared_ptr<compiled_expression<atom_type, S, 2> > compiled_expr;
    boost::shared_ptr<compiled_expression<atom_type, S, 2> > compiled_extra_expr;
    std::vector<bool> extra_independence;
    std::vector<boost::shared_ptr<tree_specification<atom_type> > > tree_specs;

    int lhs_symbol;
    std::vector<int> rhs_symbols;
    int starred_ix;

    int origin;
    // tylko w regulach lokalnych
    E equivalent;

    bool is_an_origin;
    bool is_indexed;
    bool is_prehooked;

#ifdef _T5_WITH_EXTSCORES
    bool is_extscore;
    S extscore;
#endif

    rule_holder() :
        g_rule(),
        compiled_expr(),
        compiled_extra_expr(),
        tree_specs(),
        lhs_symbol(-1L),
        starred_ix(-1L),
        origin(0),
        is_an_origin(false),
        is_indexed(false),
        is_prehooked(false)
#ifdef _T5_WITH_EXTSCORES
        ,
        is_extscore(false),
        extscore(0)
#endif
    { }

    // tworzy lisc
    rule_holder(atom_type cat, E eq = E()) :
        g_rule(),
        compiled_expr(),
        compiled_extra_expr(),
        tree_specs(),
        lhs_symbol(-1L),
        starred_ix(-1L),
        origin(0),
        equivalent(eq),
        is_an_origin(false),
        is_indexed(false),
        is_prehooked(false)
#ifdef _T5_WITH_EXTSCORES
        ,
        is_extscore(false),
        extscore(0)
#endif
    {
        tree_specs.push_back(
            boost::shared_ptr<tree_specification<atom_type> >(
                new tree_specification<atom_type>(
                    boost::shared_ptr< tree_specification_root<atom_type> >(
                        new category_root<atom_type>(cat)
                    )
                )
            )
        );
    }

    rule_holder(boost::shared_ptr<tree_specification<atom_type> > ts, E eq = E()) :
        g_rule(),
        compiled_expr(),
        compiled_extra_expr(),
        tree_specs(),
        lhs_symbol(-1L),
        starred_ix(-1L),
        origin(0),
        equivalent(eq),
        is_an_origin(false),
        is_indexed(false),
        is_prehooked(false)
#ifdef _T5_WITH_EXTSCORES
        ,
        is_extscore(false),
        extscore(0)
#endif
    {
        tree_specs.push_back(ts);
    }

    ~rule_holder() { }

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/)
        {
        ar.template register_type<empty_root<atom_type> >();
        ar.template register_type<category_root<atom_type> >();
        ar.template register_type<hook_root<atom_type> >();
        ar.template register_type<hook_contents_root<atom_type> >();

        ar & compiled_expr;
        ar & compiled_extra_expr;
        ar & extra_independence;
        ar & tree_specs;

        ar & lhs_symbol;
        ar & rhs_symbols;
        ar & starred_ix;

        ar & is_an_origin;
        ar & is_indexed;
        ar & is_prehooked;
        }
    };

    struct rule_type
    {
    int rule_ix_ : 24;
    unsigned tree_choice_ : 8;
    S score_;
    entry_type entry_;

    entry_type category()   const { return entry_; }
    S score()               const { return score_; }
    int rule_no() const { return rule_ix_; }

    rule_type(int a_rule_ix, unsigned int a_tree_choice, S a_score, const entry_type& a_entry):
        rule_ix_(a_rule_ix),
        tree_choice_(a_tree_choice),
        score_(a_score),
        entry_(a_entry)
        {}
    };

    std::pair<
        typename std::vector<rule_holder>::iterator,
        typename std::vector<rule_holder>::iterator
    > get_rules_iterator();

    std::vector<rule_type> combine_unary(const entry_type& entry);
    std::vector<rule_type> combine_binary(
    const entry_type& first_entry,
    const entry_type& second_entry);

    bool are_rules_for_catprints(
    int catprint_a,
    int catprint_b) const;

    bool could_be_final(const entry_type& entry);
    bool could_be_final_variant(const entry_type& entry, const variant_type& variant);
    bool is_lexical(const rule_type& rule) const;
    boost::shared_ptr<tree_specification<atom_type> > tree_spec(
    const rule_type& rule, const std::vector<rule_holder>& local_rules);
    E equivalent(
    const rule_type& rule, const std::vector<rule_holder>& local_rules);
    bool is_useful(const entry_type& entry) const;
    void clear_parser_caches();

    bool variant_combine_unary(
    entry_type& super_entry,
    const entry_type& sub_entry,
    const variant_type& sub_variant,
    const rule_type& rule,
    variant_type& new_variant,
    S& score);

    bool variant_combine_binary(
    entry_type& super_entry,
    const entry_type& left_sub_entry,
    const entry_type& right_sub_entry,
    const variant_type& left_sub_variant,
    const variant_type& right_sub_variant,
    const rule_type& rule,
    variant_type& new_variant,
    S& score);

    bool is_variant_independent(
    const rule_type& rule,
    int symbol_ix);

    S get_preference(atom_type syncat);

    std::string get_option(const std::string& option_name) const;

    size_t get_nb_limits() const;
    int get_limit_threshold(size_t limit_ix) const;
    S get_limit_min_score(size_t limit_ix) const;

    /**
     * Maksymalna liczba krawedzi pomiedzy dwoma wierzcholkami.
     */
    int get_limit_edges(int len) const;

    registrar<std::string>& get_attribute_registrar();
    registrar<std::string>& get_extra_attribute_registrar();
    registrar<std::string>& get_symbol_registrar();
    M& get_master();
    X& get_semantics_machine();

    bool check_rules();

    //! Wypisuje reguly parsera
    /**
     * Nie jest to format zgodny z pierwotnym zapisem regul, na przyklad
     * wyrazenia atrybutywne sa wypisywanie w "kodzie maszynowym
     */
    void print_rules(std::ostream& ostr, bool with_tree_specifications=false) const;


    // vvv METODY POTRZEBNE DO ASTAR vvv

    // znajduje mozliwych rodzicow jedynakow
    std::vector<rule_type> find_only_child_parent(const entry_type& symbol);

    // znajduje mozliwych lewych braci z rodzicami
    std::vector< std::pair<rule_type, entry_type> >
        find_left_sibling_with_parent(entry_type second_symbol);

    // znajduje mozliwych prawych braci z rodzicami
    std::vector< std::pair<rule_type, entry_type> >
        find_right_sibling_with_parent(entry_type first_symbol);

    // znajduje reguly binarne, ktorych glowa jest dany wezel
    std::vector< std::pair< rule_type, std::pair<entry_type, entry_type> > >
        split(entry_type parent_symbol);

    // znajduje reguly unarne, ktorych glowa jest dany wezel
    // z tajemniczego powodu, jesli definicja ponizszej funkcji byla w .tpl nie chcialo
    // sie kompilowac pod Visual C++
    std::vector<std::pair<rule_type, entry_type> > find_only_children(entry_type parent_entry)
    {
    std::vector< std::pair< rule_type, entry_type > > r;
    int parent_symbol_id = parent_entry.get_cat();
    assert(parent_symbol_id >= 0);
    for (
        typename std::vector< std::vector<int> >::iterator urii1 = unary_rules_index_.begin();
        urii1 != unary_rules_index_.end();
        ++urii1
    ) {
        for (
            typename std::vector<int>::iterator urii2 = (*urii1).begin();
            urii2 != (*urii1).end();
            ++urii2
        ) {
            int rule_ix = (*urii2);
            if (rules_[rule_ix].is_indexed && (rules_[rule_ix].lhs_symbol == parent_symbol_id)) {
                   compile_rule_(rule_ix);
//              assert(rules_[rule_ix].compiled_expr);
                entry_type child_entry(rules_[rule_ix].rhs_symbols[0]);
                typename expression_runner_type::runner_frame frame;
                frame.lhs = &parent_entry;
                frame.rhs.push_back(&child_entry);
                if (runner_.run(
                    *(rules_[rule_ix].compiled_expr),
                    frame,
                    master_,
                    semantics_machine_
                )) {
                    r.push_back(std::pair< rule_type, entry_type >(
                        rule_type(rule_ix, frame.chosen_tree_ix, frame.score, parent_entry),
                        child_entry
                    ));
                }
            }
        }
    }
    return r;
    }

    // dodaje wierzcholek do listy korzeni
    void set_root(entry_type state);

    // sprawdza, czy jest korzeniem
    bool is_root(entry_type state);

    // sprawdza, czy jest symbolem terminalnym
    bool is_terminal(entry_type state) {
        return ((split(state).size()==0)&&(find_only_children(state).size()==0));
    }

    // ^^^ METODY POTRZEBNE DO ASTAR ^^^


#ifdef _T5_WITH_EXTSCORES
    class incompatible_extscores : public std::runtime_error {
    public:
        incompatible_extscores() : std::runtime_error("incompatible external scores") { }
    };
#endif

private:
    // zablokowany konstruktor kopiujacy
    tgbg_combinator(const tgbg_combinator&);

    bool with_decomposition_;

    void add_rule_(GRule* g_rule);
    void index_rule_(int rule_ix);
    void index_unary_rule_(int rule_ix);
    void index_binary_rule_(int rule_ix);
    void linearize_rule_(int rule_ix);
    std::vector<rule_holder> linearize_seq_(
    const std::vector<rule_holder>& v,
    const GRuleRightSymbolsSeq* seq);
    std::vector<rule_holder> linearize_alt_(
    const std::vector<rule_holder>& v,
    const GRuleRightSymbolAltsSeq* seq);

    void decompose_rule_(int rule_ix);

    static void relocate_grule_(
    rule_holder& rh,
    int begin_attr_ix,
    int end_attr_ix,
    bool delete_star = true);

    registrar<std::string> attribute_reg_;
    registrar<std::string> extra_attribute_reg_;
    registrar<std::string> symbol_reg_;
    M master_;
    X semantics_machine_;
    std::vector<rule_holder> rules_;

    std::vector<std::vector<int> > unary_rules_index_;

    std::vector<std::vector<int>* > binary_rules_index_;
    int nb_indexed_symbols_;
    void resize_binary_rules_index_(int sid);

    int pre_hook_symbol_id_;
    boost::shared_ptr<compiled_expression<atom_type, S, 2> > pre_hook_binary_expr_;

    boost::shared_ptr<compiled_expression<atom_type, S, 2> > pre_hook_unary_expr_;
    int final_hook_symbol_id_;
    boost::shared_ptr<compiled_expression<atom_type, S, 2> > final_hook_expr_;
    boost::shared_ptr<compiled_expression<atom_type, S, 2> > final_hook_extra_expr_;

    int extra_attributes_symbol_id_;

    int setscore_factor_id_;
    S setscore_factor_;

    int preference_id_;
    std::map<atom_type, S> preference_table_;

    int options_id_;
    std::map<std::string, std::string> options_;

    int limits_id_;
    struct limits_entry
    {
    int threshold;
    S min_score;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/)
        {
        ar & threshold;
        ar & min_score;
        }

    };
    std::vector<limits_entry> limits_;

    void check_special_rule_(int rule_ix);
    void read_score_encoded_in_special_rule_(int rule_ix, S& encoded_value);
    void apply_hooks_(int rule_ix);

    typedef expression_runner<BaseCategory, atom_type, S, M, X, true> expression_runner_type;

    expression_runner_type runner_;

    void compile_rule_(int rule_ix);
    void print_rule_(std::ostream& ostr, size_t rule_ix, bool with_tree_specifications) const;

    // skrot "bid" oznacza pare symboli ktore polaczyc w jeden sztuczny symbol "<...!...>"

    // miejsce pary symboli w regule
    struct bid_ref
    {
    typename std::list<rule_holder>::iterator rule_it;
    // indeks lewego symbolu w regule
    int symbol_ix;
    // atrybuty, ktore trzeba przepchac do sztucznego symbolu
    std::set<int> attrs;
    // to samo dla extra-atrybutow
    std::set<int> extra_attrs;

    bid_ref(
        typename std::list<rule_holder>::iterator a_rule_it,
        int a_symbol_ix):
        rule_it(a_rule_it),
        symbol_ix(a_symbol_ix)
        {}
    };

    struct bid_info
    {
    std::list<bid_ref> refs;
    std::multiset<int> attrs;
    std::multiset<int> extra_attrs;
    };

    // para symboli jest reprezentowana przez ((l,r),g), gdzie
    // l - lewy symbol, r - prawy symbol, g - 0 lub 1 (ktory jest glowny)
    // a zatem ((l,r),0) i ((l,r),1) to rozne bid-y!
    typedef std::pair<std::pair<int, int>, int> bid_key;

    void gather_bids_(
    typename std::list<rule_holder>::iterator rule_it,
    typename HashWrapper<bid_key, bid_info>::type& bids);

    typename HashWrapper<
        typename tgbg_combinator<T, S, M, X, E>::bid_key,
        typename tgbg_combinator<T, S, M, X, E>::bid_info
    >::type::iterator best_bid_(typename HashWrapper<bid_key, bid_info>::type& bids);

    int get_bid_symbol_(const bid_key& k);

    void remove_bid_ref_(
    typename HashWrapper<bid_key, bid_info>::type& bids,
    typename std::list<rule_holder>::iterator rule_it,
    int symbol_ix,
    int d,
    const bid_key& forbidden);

    void move_bid_ref_(
    typename HashWrapper<bid_key, bid_info>::type& bids,
    typename std::list<rule_holder>::iterator rule_it,
    int symbol_ix,
    int d,
    const bid_key& forbidden);

    void add_bid_(
    typename HashWrapper<bid_key, bid_info>::type& bids,
    typename std::list<rule_holder>::iterator rule_it,
    int symbol_ix,
    int d);

    static boost::shared_ptr<compiled_expression<atom_type, S, 2> >
    get_assignments_expression_(
    registrar<std::string>& attr_registrar,
    const std::string& prefix,
    const std::multiset<int>&,
    int starred_ix,
    int already_reserved_for_temps,
    int & reserved_for_temps,
    int settop_subvar_opcode,
    int assign_opcode,
    bool could_be_null=false);

    static int get_temp_attribute_(
    registrar<std::string>& attr_registrar,
    const std::string& prefix,
    int ix);

    static void get_attrs_(
    std::set<int>& s, const compiled_expression<atom_type, S, 2>& expr, int symbol_ix);
    static void get_extra_attrs_(
    std::set<int>& s, const compiled_expression<atom_type, S, 2>& expr, int symbol_ix);

    static void create_reindex_hash_(
    registrar<std::string>& attr_registrar,
    const std::string& prefix,
    const std::multiset<int> & attrs_mset,
    typename HashWrapper<int, int>::type & r_hash,
    int already_reserved_for_temps);

    boost::shared_ptr<tree_specification<atom_type> > empty_tree_spec_;

    int last_useful_symbol_id_;

    int get_filtre_id_(int symbol_ix, int filtre_ix);

    int last_filtre_ix_;
    typename HashWrapper3<
    std::pair<int, compiled_expression<atom_type, S, 2> >,
    int,
    int_compiled_expression_hash_fun<atom_type, S, 2> >::type
    filtres_hash_;

    friend class boost::serialization::access;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const;

    template<class Archive>
    void load(Archive & ar, const unsigned int version);

#ifdef _T5_WITH_EXTSCORES
    void apply_extscores_(
        typename std::vector<rule_holder>::iterator it,
        typename rule_score_reader<S>::rule_scores_t& rst);

    void try_using_extscore_(
        const rule_holder& rule,
        typename expression_runner_type::runner_frame&);
#endif

    // klasa rzutujaca ten kombinator musi byc zaprzyjazniona
    friend class tgbg_to_simple_cfg_projector<atom_type, S, M, X, E>;
    friend class tgbg_to_tab_int_projector<atom_type, S, M, X, E>;
    friend class tgbg_attribute_projector<atom_type, S, M, X, E>;

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

#endif
