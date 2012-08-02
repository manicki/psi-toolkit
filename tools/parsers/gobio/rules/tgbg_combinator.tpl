#ifndef TGBG_COMBINATOR_TPL_HDR
#define TGBG_COMBINATOR_TPL_HDR

#include <algorithm>
#include <fstream>
#include <limits>
#include <sstream>

#include "compile_gruleexpression.tpl"
#include "convert_tree_recipe.tpl"
#include "decompose_expression.tpl"
#include "default_tree_spec.tpl"
#include "disassemble_expression.tpl"
#include "expression_runner.tpl"
#include "grparser.h"
#include "grule_helpers.hpp"
#include "opcodes.hpp"
#include "relocate_expression.tpl"
#include "tgbg_combinator.hpp"
#include "tree_specification.tpl"

#ifdef _T5_WITH_EXTSCORES
#include "gen_ruledumper.tpl"
#endif


template<class S>
class extra_numeric_limits
{
public:
    static S min_value()
    {
    return
        (std::numeric_limits<S>::is_integer
         ? std::numeric_limits<S>::min()
         : -std::numeric_limits<S>::max());
    }
};


template<class T, class S, class M, class X, class E>
tgbg_combinator<T, S, M, X, E>::tgbg_combinator(M & master, bool with_decomposition):
    with_decomposition_(with_decomposition),
    master_(master),
    semantics_machine_(master_),
    pre_hook_symbol_id_(-1L),
    final_hook_symbol_id_(-1L),
    extra_attributes_symbol_id_(-1L),
    setscore_factor_id_(-1L),
    setscore_factor_(1),
    preference_id_(-1L),
    options_id_(-1L),
    limits_id_(-1L),
    last_useful_symbol_id_(-1L),
    last_filtre_ix_(0)
{
    rules_.reserve(1024);

    unary_rules_index_.resize(128);

    nb_indexed_symbols_ = 128;
    binary_rules_index_.resize(nb_indexed_symbols_*nb_indexed_symbols_);

    pre_hook_symbol_id_ = symbol_reg_.get_id("__PREHOOK__");
    final_hook_symbol_id_ = symbol_reg_.get_id("__FINALHOOK__");
    extra_attributes_symbol_id_ = symbol_reg_.get_id("__EXTRA_ATTRIBUTES__");
    setscore_factor_id_ = symbol_reg_.get_id("__SETSCORE_FACTOR__");
    preference_id_ = symbol_reg_.get_id("__PREFERENCE__");

    empty_tree_spec_.reset(new tree_specification<atom_type>(
                   boost::shared_ptr<tree_specification_root<atom_type> >(
                   new empty_root<atom_type>())));

    options_id_ = symbol_reg_.get_id("__OPTIONS__");

    limits_id_ = symbol_reg_.get_id("__LIMITS__");
    limits_.push_back(limits_entry());
    limits_.back().threshold = std::numeric_limits<int>::max();
    limits_.back().min_score = extra_numeric_limits<S>::min_value();
}

template<class T, class S, class M, class X, class E>
tgbg_combinator<T, S, M, X, E>::~tgbg_combinator()
{
    for (unsigned int i=0;i<binary_rules_index_.size();++i)
    delete binary_rules_index_[i];
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::add_rules(std::istream& rules_stream)
{
    GRParser G;

    GRuleList* rules = G.parse(rules_stream, false);

    for (unsigned int i=0;i<rules->size();++i)
    {
    add_rule_((*rules)[i]);
    (*rules)[i] = 0;

    check_special_rule_(rules_.size()-1);
    }

    delete rules;

    decompose_all_rules();
    binarize_all_rules();
    index_all_rules();

    last_useful_symbol_id_ = symbol_reg_.last_id();
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::add_rules(const std::string& filename)
{
    std::ifstream fs(filename.c_str());

    add_rules(fs);
}


#ifdef _T5_WITH_EXTSCORES
template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::read_extscores(rule_score_reader<S>* rs_reader)
{

std::cerr << "Reading extscores..." << std::flush;

    if (rs_reader->is_empty()) {
std::cerr << " SCORE READER IS EMPTY!" << std::endl;
        return;
    }

    gen_ruledumper<atom_type, S, M, X, E> grd;

    std::pair<typename std::vector<rule_holder>::iterator,typename std::vector<rule_holder>::iterator> itp
        = get_rules_iterator();

    typename std::vector<rule_holder>::iterator it = itp.first;

    while (it != itp.second)
    {
        if (!(*it).is_an_origin)
        {
            std::string ruleprint = grd.get_signature(*it);

            typename rule_score_reader<S>::rule_scores_t rst = rs_reader->get_scores(ruleprint);

#if CLASSIC_EXTSCORES
            if (!rst.empty())
                apply_extscores_(it, rst);
#else
            if (!rst.empty()) {
                apply_extscores_(it, rst);
            } else {
                (*it).compiled_expr->set_score(-1000);
            }
#endif
        }

        ++it;
    }

std::cerr << " DONE" << std::endl;

}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::apply_extscores_(
    typename std::vector<rule_holder>::iterator it,
    typename rule_score_reader<S>::rule_scores_t& rst)
{
    for (typename rule_score_reader<S>::rule_scores_t::iterator rst_it = rst.begin();
         rst_it != rst.end();
         ++rst_it)
    {
        int ix = (*rst_it).first;
        S single_score = (*rst_it).second;

        if (ix == -1L)
        {
            (*it).is_extscore = true;
            (*it).extscore = single_score;
        }
        else
        {
#if CLASSIC_EXTSCORES
            if ((*it).compiled_expr &&
                (*it).compiled_expr->get_opcode(ix) == gobio_opcodes::OPCODE_SET_SCORE)
                (*it).compiled_expr->set_farg(ix, 0, single_score);
            else
                throw incompatible_extscores();
#else
            if (!((*it).compiled_expr)) {
                throw incompatible_extscores();
            } else {
                (*it).compiled_expr->set_score(single_score);
            }
#endif
        }
    }
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::try_using_extscore_(
    const rule_holder& rule,
    typename expression_runner_type::runner_frame& frame)
{
    if (rule.is_extscore)
        frame.score = rule.extscore;
}
#endif


//ZZZ
template<class T, class S, class M, class X, class E>
std::pair<typename std::vector<typename tgbg_combinator<T, S, M, X, E>::rule_holder>::iterator,typename std::vector<typename tgbg_combinator<T, S, M, X, E>::rule_holder>::iterator>
 tgbg_combinator<T, S, M, X, E>::get_rules_iterator()
{
return std::pair<typename std::vector<rule_holder>::iterator,typename std::vector<rule_holder>::iterator>(rules_.begin(),rules_.end());
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::compile_all_rules()
{
    for (int i=0;i < (signed int)rules_.size();++i)
    compile_rule_(i);
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::index_all_rules()
{
    for (int i=0;i < (signed int)rules_.size();++i)
    index_rule_(i);
}

template<class T, class S, class M, class X, class E>
void clear_is_indexed_for_rule_holder_(typename tgbg_combinator<T, S, M, X, E>::rule_holder& rh)
{
    rh.is_indexed = false;
}

template<class T, class S, class M, class X, class E>
void clear_unary_index_element_(std::vector<int>& uie)
{
    uie.clear();
}

template<class T, class S, class M, class X, class E>
void clear_binary_index_element_(std::vector<int>*& bie)
{
    delete bie;
    bie = 0;
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::clear_indexes()
{
    std::for_each(
    rules_.begin(),
    rules_.end(),
    clear_is_indexed_for_rule_holder_<T, S, M, X, E>);

    std::for_each(
    unary_rules_index_.begin(),
    unary_rules_index_.end(),
    clear_unary_index_element_<T, S, M, X, E>);

    std::for_each(
    binary_rules_index_.begin(),
    binary_rules_index_.end(),
    clear_binary_index_element_<T, S, M, X, E>);
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::binarize_all_rules()
{
    typename HashWrapper<bid_key, bid_info>::type bids;

    std::list<typename tgbg_combinator::rule_holder> long_rules;

    // w tym haszu będziemy przechowywać, ile atrybutow tymczasowych ("!Tn")
    // zostało już wykorzystanych dla danego symbolu pomocniczego; jest to ważne,
    // by nie pomieszały sie atrybuty tymczasowymi lewego podskładnika z
    // atrybutami tymczasowymi prawego podskładnika
    typename HashWrapper<int, int>::type reserved_for_temps_hash;

    // i to samo dla extra-atrybutów
    typename HashWrapper<int, int>::type reserved_for_extra_temps_hash;

    // zbieramy informacje o "bid"-ach
    for (int i=0;i < (signed int)rules_.size(); ++i)
    {
    rule_holder& h = rules_[i];

    if (!h.is_an_origin && h.rhs_symbols.size() > 2)
    {
        compile_rule_(i);
        long_rules.push_back(h);

        typename std::list<typename tgbg_combinator::rule_holder>::iterator last_it = long_rules.end();
        --last_it;

        (*last_it).origin = i;
        (*last_it).g_rule.reset();
        gather_bids_(last_it, bids);
        h.is_an_origin = true;

        assert(!(*last_it).is_indexed);
    }
    }

    while (!bids.empty())
    {
    typename HashWrapper<
        bid_key,
        bid_info>::type::iterator max_it =
        best_bid_(bids);

    std::list<bid_ref>& refs = (*max_it).second.refs;

    const int d = (*max_it).first.second;

    const int already_reserved_for_temps =
        (d ?
         (reserved_for_temps_hash.count((*max_it).first.first.second) ?
          reserved_for_temps_hash[(*max_it).first.first.second] :
          0) :
         (reserved_for_temps_hash.count((*max_it).first.first.first) ?
          reserved_for_temps_hash[(*max_it).first.first.first] :
          0));

    typename HashWrapper<int, int>::type attrs_to_ts_hash;
    create_reindex_hash_(
        attribute_reg_,
        "!T",
        (*max_it).second.attrs,
        attrs_to_ts_hash,
        already_reserved_for_temps);

    // jw. dla extra-atrybutów
    const int already_reserved_for_extra_temps =
        (d ?
         (reserved_for_extra_temps_hash.count((*max_it).first.first.second) ?
          reserved_for_extra_temps_hash[(*max_it).first.first.second] :
          0) :
         (reserved_for_extra_temps_hash.count((*max_it).first.first.first) ?
          reserved_for_extra_temps_hash[(*max_it).first.first.first] :
          0));

    typename HashWrapper<int, int>::type extra_attrs_to_ts_hash;
    create_reindex_hash_(
        extra_attribute_reg_,
        "!X",
        (*max_it).second.extra_attrs,
        extra_attrs_to_ts_hash,
        already_reserved_for_extra_temps);


    // dodanie reguły binarnej
    const int bid_symbol = get_bid_symbol_((*max_it).first);
    assert(!refs.empty());
    rules_.push_back(rule_holder());
    size_t added_bi_rule_ix = rules_.size() - 1;
    int reserved_for_temps = -1L;
    rules_.back().compiled_expr = get_assignments_expression_(
        attribute_reg_,
        "!T",
        (*max_it).second.attrs,
        d,
        already_reserved_for_temps,
        reserved_for_temps,
        gobio_opcodes::OPCODE_SETTOP_SUBVAR,
        gobio_opcodes::OPCODE_ASSIGN);

    assert(reserved_for_temps >= already_reserved_for_temps);

    int reserved_for_extra_temps = -1L;
    rules_.back().compiled_extra_expr = get_assignments_expression_(
        extra_attribute_reg_,
        "!X",
        (*max_it).second.extra_attrs,
        d,
        already_reserved_for_extra_temps,
        reserved_for_extra_temps,
        gobio_opcodes::OPCODE_SETTOP_EXTRA_SUBVAR,
        gobio_opcodes::OPCODE_EXTRA_ASSIGN,
        true);


    rules_.back().extra_independence = get_independence_vector<atom_type,S>(
        rules_.back().compiled_extra_expr,
        2,
        d);

    assert(reserved_for_extra_temps >= already_reserved_for_extra_temps);

    reserved_for_temps_hash[bid_symbol] = reserved_for_temps;
    reserved_for_extra_temps_hash[bid_symbol] = reserved_for_extra_temps;

    rules_.back().tree_specs.push_back(default_tree_spec<atom_type>(2, -1L, master_));

    rules_.back().lhs_symbol = bid_symbol;
    rules_.back().rhs_symbols.push_back((*max_it).first.first.first);
    rules_.back().rhs_symbols.push_back((*max_it).first.first.second);
    rules_.back().starred_ix = d;


    unsigned int nb_of_rules = 0;
    typename HashWrapper3<
        compiled_expression<atom_type, S, 2>,
        int,
        compiled_expression_hash_fun<atom_type, S, 2> >::type bifs_hash;



    // wstawianie nowego sztucznego symbolu po prawych stronach reguł
    for (typename std::list<bid_ref>::iterator
        r_it = refs.begin();
        r_it != refs.end();
        ++r_it)
    {
        const typename std::list<rule_holder>::iterator rule_it
        = (*r_it).rule_it;
        const int symbol_ix = (*r_it).symbol_ix;

        if (symbol_ix < 0)
        {
        assert(symbol_ix == -1L);
        continue;
        }

        assert(symbol_ix >= 0 && symbol_ix < (signed int)((*rule_it).rhs_symbols.size() - 1));

        // sprawdzenie, czy nie mozna jakichs filtrow nalozyc
        std::vector<boost::shared_ptr<compiled_expression<atom_type, S, 2> > > bifs
        = get_bifiltres(
            (*rule_it).compiled_expr,
            (*rule_it).rhs_symbols.size(),
            (*rule_it).starred_ix,
            symbol_ix);

        for (size_t i = 0; i < bifs.size(); ++i)
        ++bifs_hash[*bifs[i]];

        ++nb_of_rules;

        // --------------------------------------------------

        remove_bid_ref_(bids, rule_it, symbol_ix, 1 - d, (*max_it).first);

        if (symbol_ix > 0)
        {
        for (int d = 0; d <= 1; ++d)
            remove_bid_ref_(bids, rule_it, symbol_ix - 1, d, (*max_it).first);
        }

        if (symbol_ix < (signed int)(*rule_it).rhs_symbols.size() - 2)
        {
        for (int d = 0; d <= 1; ++d)
            remove_bid_ref_(bids, rule_it, symbol_ix + 1, d, (*max_it).first);
        }

        for (size_t i = (unsigned int)(symbol_ix + 2);
        i < (*rule_it).rhs_symbols.size() - 1;
        ++i)
        {
        for (int d = 0; d <= 1; ++d)
            move_bid_ref_(bids, rule_it, i, d, (*max_it).first);
        }


        {
        typename std::list<bid_ref>::iterator c_it
            = r_it;
        ++c_it;
        // ta sama para może być w jednej regule
        for (;c_it != refs.end();++c_it)
        {
            if ((*c_it).rule_it == (*r_it).rule_it)
            {
            assert((*c_it).symbol_ix > (*r_it).symbol_ix);

            --(*c_it).symbol_ix;

            if ((*c_it).symbol_ix == (*r_it).symbol_ix)
                (*c_it).symbol_ix = -1L;
            }
        }
        }

        // wstawienie binarnego symbolu
        assert(symbol_ix >= 0 && symbol_ix < (signed int)(*rule_it).rhs_symbols.size());
        (*rule_it).rhs_symbols.erase((*rule_it).rhs_symbols.begin() + symbol_ix);
        (*rule_it).rhs_symbols[symbol_ix] = bid_symbol;

        // poprawiamy normalne atrybuty

        (*rule_it).compiled_expr.reset(
        new compiled_expression<atom_type, S, 2>(*(*rule_it).compiled_expr));
        reindex_attributes(
        *(*rule_it).compiled_expr,
        symbol_ix + (1 - d),
        symbol_ix,
        attrs_to_ts_hash,
        gobio_opcodes::OPCODE_PUSH_SUBVAR,
        gobio_opcodes::OPCODE_SETTOP_SUBVAR);
        if ((*rule_it).compiled_extra_expr)
        reindex_attributes(
            *(*rule_it).compiled_extra_expr,
            symbol_ix + (1 - d),
            symbol_ix,
            attrs_to_ts_hash,
            gobio_opcodes::OPCODE_PUSH_SUBVAR,
            gobio_opcodes::OPCODE_SETTOP_SUBVAR);
        // jeśli d == 1, trzeba przesunąć atrybuty, żeby relocate_grule_
        // poniżej nie wpisało PUSH FALSE
        if (d)
        {
        relocate_attributes_from_symbol_to_symbol(
            *(*rule_it).compiled_expr,
            symbol_ix + 1,
            symbol_ix,
            gobio_opcodes::OPCODE_PUSH_SUBVAR,
            gobio_opcodes::OPCODE_SETTOP_SUBVAR);

        if ((*rule_it).compiled_extra_expr)
            relocate_attributes_from_symbol_to_symbol(
            *(*rule_it).compiled_extra_expr,
            symbol_ix + 1,
            symbol_ix,
            gobio_opcodes::OPCODE_PUSH_SUBVAR,
            gobio_opcodes::OPCODE_SETTOP_SUBVAR);
        }

        // ... a teraz poprawiamy ekstra-atrybuty

        if ((*rule_it).compiled_extra_expr)
        {
        (*rule_it).compiled_extra_expr.reset(
            new compiled_expression<atom_type, S, 2>(*(*rule_it).compiled_extra_expr));
        reindex_attributes(
            *(*rule_it).compiled_extra_expr,
            symbol_ix + (1 - d),
            symbol_ix,
            extra_attrs_to_ts_hash,
            gobio_opcodes::OPCODE_PUSH_EXTRA_SUBVAR,
            gobio_opcodes::OPCODE_SETTOP_EXTRA_SUBVAR);
        if (d)
            relocate_attributes_from_symbol_to_symbol(
             *(*rule_it).compiled_extra_expr,
             symbol_ix + 1,
             symbol_ix,
             gobio_opcodes::OPCODE_PUSH_EXTRA_SUBVAR,
             gobio_opcodes::OPCODE_SETTOP_EXTRA_SUBVAR);
         }

        // poprawiamy gwiazdkę

        if ((*rule_it).starred_ix == symbol_ix + 1)
        --(*rule_it).starred_ix;

        // i drzewa
        for (typename std::vector<boost::shared_ptr<tree_specification<atom_type> > >::iterator ts_it
            = (*rule_it).tree_specs.begin();
        ts_it != (*rule_it).tree_specs.end();
        ++ts_it)
        {
        (*ts_it) = (*ts_it)->clone();
        (*ts_it)->move_hook_down(symbol_ix, symbol_ix, 0);
        (*ts_it)->move_hook_down(symbol_ix + 1, symbol_ix, 1);
        }
        relocate_grule_(*rule_it, symbol_ix + 1, symbol_ix + 2, false);

        if ((*rule_it).rhs_symbols.size() == 2)
        {
        rules_.push_back(*rule_it);
        long_rules.erase(rule_it);
        }
        else
        {
        assert((*rule_it).rhs_symbols.size() > 2);

        if (symbol_ix > 0)
        {
            for (int d = 0; d <= 1; ++d)
            add_bid_(bids, rule_it, symbol_ix - 1, d);
        }

        if (symbol_ix < (signed int)((*rule_it).rhs_symbols.size() - 1))
        {
            for (int d = 0; d <= 1; ++d)
            add_bid_(bids, rule_it, symbol_ix, d);
        }
        }
    }

    // badamy filtry
    typename HashWrapper3<
        compiled_expression<atom_type, S, 2>,
        int,
        compiled_expression_hash_fun<atom_type, S, 2> >::type::iterator bifs_hash_it
        = bifs_hash.begin();

    bool was_filtre_added = false;
    bool first_filtre = true;

    for ( ; bifs_hash_it != bifs_hash.end(); ++bifs_hash_it)
        if ((*bifs_hash_it).second == (int)(nb_of_rules))
        {
        if (first_filtre)
        {
            rules_[added_bi_rule_ix].compiled_expr =
            push2settop(*rules_[added_bi_rule_ix].compiled_expr);

            first_filtre = false;
        }

        was_filtre_added = true;

        boost::shared_ptr<compiled_expression<atom_type, S, 2> > f_expr
            = paste_expressions_with_ifnjump<atom_type,S>(
            (*bifs_hash_it).first, *rules_[added_bi_rule_ix].compiled_expr);

        rules_[added_bi_rule_ix].compiled_expr
            = f_expr;
        }

    if (was_filtre_added)
        rules_[added_bi_rule_ix].compiled_expr =
        settop2push(*rules_[added_bi_rule_ix].compiled_expr);

    // ------------------


    // bids.erase(max_it); //replaced by the following line to avoid memory access violation errors
    bids.erase(max_it->first);

    }
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::decompose_all_rules()
{
    if (!with_decomposition_)
    return;

    // w petli uzywamy tej wartosci, żeby nie rozkładać
    // reguł, które powstały w wyniku rozkładu
    // innej reguły!
    size_t nb_rules = rules_.size();

    for (int i=0;i < (signed int)nb_rules; ++i)
    {
    if (!rules_[i].is_an_origin && rules_[i].rhs_symbols.size() > 1)
    {
        compile_rule_(i);
        decompose_rule_(i);
    }
    }
}

template<class T, class S, class M, class X, class E>
bool is_purgeable(const typename tgbg_combinator<T, S, M, X, E>::rule_holder& rh)
{
    return rh.is_an_origin;
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::purge_rules()
{
    clear_indexes();

    rules_.erase(
    std::remove_if(
        rules_.begin(),
        rules_.end(),
        is_purgeable<T, S, M, X, E>),
    rules_.end());

    index_all_rules();
    last_useful_symbol_id_ = symbol_reg_.last_id();
}

template<class T, class S, class M, class X, class E>
int tgbg_combinator<T, S, M, X, E>::get_bid_symbol_(
    const bid_key& k)
{
    return symbol_reg_.get_id(
    std::string("<")
    + symbol_reg_.get_obj(k.first.first)
    + std::string("!")
    + symbol_reg_.get_obj(k.first.second)
    + std::string("!")
    + (k.second ? "1" : "0")
    + std::string(">"));
}

template<class T, class S, class M, class X, class E>
typename HashWrapper<typename tgbg_combinator<T, S, M, X, E>::bid_key,
                      typename tgbg_combinator<T, S, M, X, E>::bid_info>::type::iterator
tgbg_combinator<T, S, M, X, E>::best_bid_(
    typename HashWrapper<bid_key, bid_info>::type& bids)
{
    int max_freq = -1L;
    unsigned int min_attrs = 0;
    int min_order_force = 0;

    typename HashWrapper<
    bid_key,
    bid_info>::type::iterator best_it = bids.begin();

    // szukamy najczęstszej pary
    for (typename HashWrapper<
        bid_key,
        bid_info>::type::iterator it = bids.begin();
    it != bids.end();
    ++it)
    {
    int order_force =
        (*it).first.first.first
        + (*it).first.first.second
        + (*it).first.second;

    if ((signed int)((*it).second.refs.size()) > max_freq ||

       ((signed int)((*it).second.refs.size()) == max_freq
       && (*it).second.attrs.size() < min_attrs) ||

       ((signed int)((*it).second.refs.size()) == max_freq
       && (*it).second.attrs.size() == min_attrs
       && order_force < min_order_force))
    {
        max_freq = (signed int)(*it).second.refs.size();
        min_attrs = (*it).second.attrs.size();
        min_order_force = order_force;
        best_it = it;
    }
    }

    return best_it;
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::remove_bid_ref_(
    typename HashWrapper<
    bid_key,
    bid_info>::type& bids,
    typename std::list<rule_holder>::iterator rule_it,
    int symbol_ix,
    int d,
    const bid_key& forbidden)
{
    assert(symbol_ix >= 0 && symbol_ix < (signed int)((*rule_it).rhs_symbols.size() - 1));
    assert(d == 0 || d == 1);

    const int left_symbol_id = (*rule_it).rhs_symbols[symbol_ix];
    const int right_symbol_id = (*rule_it).rhs_symbols[symbol_ix+1];

    if (left_symbol_id == forbidden.first.first &&
       right_symbol_id == forbidden.first.second &&
       d == forbidden.second)
    return;

    bid_key k(
    std::pair<int, int>(
        left_symbol_id,
        right_symbol_id),
    d);

    if (bids.count(k))
    {
    bid_info& b = bids[k];
    for (typename std::list<bid_ref>::iterator
        r_it = b.refs.begin();
        r_it != b.refs.end();
        ++r_it)
    {
        if ((*r_it).rule_it == rule_it
           && (*r_it).symbol_ix == symbol_ix)
        {
        for (std::set<int>::iterator a_it = (*r_it).attrs.begin();
            a_it != (*r_it).attrs.end();
            ++a_it)
        {
            assert(b.attrs.count(*a_it));
            b.attrs.erase(b.attrs.find(*a_it));
        }

        for (std::set<int>::iterator a_it = (*r_it).extra_attrs.begin();
            a_it != (*r_it).extra_attrs.end();
            ++a_it)
        {
            assert(b.extra_attrs.count(*a_it));
            b.extra_attrs.erase(b.extra_attrs.find(*a_it));
        }

        b.refs.erase(r_it);
        break;
        }
    }

    if (b.refs.empty())
        bids.erase(k);
    }
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::move_bid_ref_(
    typename HashWrapper<
    bid_key,
    bid_info>::type& bids,
    typename std::list<rule_holder>::iterator rule_it,
    int symbol_ix,
    int d,
    const bid_key& forbidden)
{
    assert(symbol_ix >= 0 && symbol_ix < (signed int)((*rule_it).rhs_symbols.size() - 1));
    assert(d == 0 || d == 1);

    const int left_symbol_id = (*rule_it).rhs_symbols[symbol_ix];
    const int right_symbol_id = (*rule_it).rhs_symbols[symbol_ix+1];

    if (left_symbol_id == forbidden.first.first &&
       right_symbol_id == forbidden.first.second &&
       d == forbidden.second)
    return;

    bid_key k(
    std::pair<int, int>(
        left_symbol_id,
        right_symbol_id),
    d);

    if (bids.count(k))
    {
    bid_info& b = bids[k];
    for (typename std::list<bid_ref>::iterator
        r_it = b.refs.begin();
        r_it != b.refs.end();
        ++r_it)
    {
        if ((*r_it).rule_it == rule_it
           && (*r_it).symbol_ix == symbol_ix)
        {
        --((*r_it).symbol_ix);
        break;
        }
    }
    }
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::add_bid_(
    typename HashWrapper<
    bid_key,
    bid_info>::type& bids,
    typename std::list<rule_holder>::iterator rule_it,
    int symbol_ix,
    int d)
{
    assert(d == 0 || d == 1);
    assert(symbol_ix >= 0 && symbol_ix < (signed int)((*rule_it).rhs_symbols.size() - 1));
    assert((*rule_it).compiled_expr);

    if ((*rule_it).starred_ix >= 0 &&
       (*rule_it).starred_ix == symbol_ix + (1-d))
    return;

    const bid_key k(std::pair<int, int>(
            (*rule_it).rhs_symbols[symbol_ix],
            (*rule_it).rhs_symbols[symbol_ix+1]),
            d);

    bid_info& b = bids[k];

    b.refs.push_back(
    bid_ref(
        rule_it,
        symbol_ix));

    get_attrs_(b.refs.back().attrs, *(*rule_it).compiled_expr, symbol_ix + (1-d));
    if ((*rule_it).compiled_extra_expr)
    {
    get_attrs_(b.refs.back().attrs, *(*rule_it).compiled_extra_expr, symbol_ix + (1-d));
    get_extra_attrs_(
        b.refs.back().extra_attrs, *(*rule_it).compiled_extra_expr, symbol_ix + (1-d));
    }

    for (std::set<int>::iterator a_it = b.refs.back().attrs.begin();
    a_it != b.refs.back().attrs.end();
    ++a_it)
    b.attrs.insert(*a_it);

    for (std::set<int>::iterator a_it = b.refs.back().extra_attrs.begin();
    a_it != b.refs.back().extra_attrs.end();
    ++a_it)
    b.extra_attrs.insert(*a_it);
}

template<class T, class S, class M, class X, class E>
boost::shared_ptr<compiled_expression<T, S, 2> >
tgbg_combinator<T, S, M, X, E>::get_assignments_expression_(
    registrar<std::string>& attr_registrar,
    const std::string& prefix,
    const std::multiset<int>& attrs,
    int starred_ix,
    int already_reserved_for_temps,
    int & reserved_for_temps,
    int settop_subvar_opcode,
    int assign_opcode,
    bool could_be_null)
{
    assert(starred_ix == 0 || starred_ix == 1);

    if (could_be_null && attrs.empty())
    {
    reserved_for_temps = already_reserved_for_temps;
    return boost::shared_ptr<compiled_expression<atom_type, S, 2> >();
    }

    boost::shared_ptr<compiled_expression<atom_type, S, 2> > expr(new compiled_expression<atom_type, S, 2>());

    expr->push_instruction_0(gobio_opcodes::OPCODE_PUSH_TRUE);

    int temp_ix = already_reserved_for_temps;
    int prev_ix = -1L;

    for (std::multiset<int>::const_iterator it = attrs.begin();
    it != attrs.end();
    ++it)
    {
    if ((*it) != prev_ix)
    {
        expr->push_instruction_2ii(settop_subvar_opcode, (*it), 1 - starred_ix);
        expr->push_instruction_1i(
        assign_opcode,
        get_temp_attribute_(attr_registrar, prefix, temp_ix++));
        expr->push_instruction_0(gobio_opcodes::OPCODE_ALWAYS_TRUE);

        prev_ix = (*it);
    }
    }

    expr->push_instruction_0(gobio_opcodes::OPCODE_STOP);

    reserved_for_temps = temp_ix;

    return expr;
}

template<class T, class S, class M, class X, class E>
int tgbg_combinator<T, S, M, X, E>::get_temp_attribute_(
    registrar<std::string>& attr_registrar,
    const std::string& prefix,
    int ix)
{
    std::ostringstream osstr;
    osstr << ix;
    return attr_registrar.get_id(prefix + osstr.str());
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::get_attrs_(
    std::set<int>& s,
    const compiled_expression<T, S, 2>& expr,
    int symbol_ix)
{
    assert(symbol_ix >= 0);

    for (int i = 0; i < expr.first_available_address(); ++i)
    if ((expr.get_opcode(i) == gobio_opcodes::OPCODE_PUSH_SUBVAR ||
        expr.get_opcode(i) == gobio_opcodes::OPCODE_SETTOP_SUBVAR)
       && expr.get_iarg(i, 1) == symbol_ix)
        s.insert(expr.get_iarg(i,0));
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::get_extra_attrs_(
    std::set<int>& s,
    const compiled_expression<T, S, 2>& expr,
    int symbol_ix)
{
    assert(symbol_ix >= 0);

    for (int i = 0; i < expr.first_available_address(); ++i)
    if ((expr.get_opcode(i) == gobio_opcodes::OPCODE_PUSH_EXTRA_SUBVAR ||
        expr.get_opcode(i) == gobio_opcodes::OPCODE_SETTOP_EXTRA_SUBVAR)
       && expr.get_iarg(i, 1) == symbol_ix)
        s.insert(expr.get_iarg(i,0));
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::create_reindex_hash_(
    registrar<std::string>& attr_registrar,
    const std::string& prefix,
    const std::multiset<int> & attrs_mset,
    typename HashWrapper<int, int>::type & r_hash,
    int already_reserved_for_temps)
{
    int temp_ix = already_reserved_for_temps;
    int prev_ix = -1L;

    for (std::multiset<int>::const_iterator it = attrs_mset.begin();
    it != attrs_mset.end();
    ++it)
    {
    if ((*it) != prev_ix)
    {
        r_hash[*it] = get_temp_attribute_(
        attr_registrar, prefix, temp_ix++);
        prev_ix = (*it);
    }
    }
}



template<class T, class S, class M, class X, class E>
std::vector<typename tgbg_combinator<T, S, M, X, E>::rule_type>
    tgbg_combinator<T, S, M, X, E>::combine_unary(
    const entry_type& entry)
{
    std::vector<rule_type> r;


    int symbol_id = entry.get_cat();

    assert(symbol_id >= 0);

    if (symbol_id < (signed int)unary_rules_index_.size())
    {
    std::vector<int>::iterator end_iter = unary_rules_index_[symbol_id].end();
    for (std::vector<int>::iterator iter = unary_rules_index_[symbol_id].begin();
        iter != end_iter;
        ++iter)
    {
        int rule_ix = (*iter);

        compile_rule_(rule_ix);
        assert(rules_[rule_ix].compiled_expr);

        entry_type new_entry(rules_[rule_ix].lhs_symbol);

        if (rules_[rule_ix].starred_ix == 0)
        new_entry.copy_attrs(entry);

        typename expression_runner_type::runner_frame frame;
        frame.lhs = &new_entry;
        frame.rhs.push_back(&entry);

        if (runner_.run(
           *(rules_[rule_ix].compiled_expr),
           frame,
           master_,
           semantics_machine_))
            {
#ifdef _T5_WITH_EXTSCORES
                try_using_extscore_(rules_[rule_ix], frame);
#endif
        r.push_back(rule_type(rule_ix, frame.chosen_tree_ix, frame.score, new_entry));
            }
    }
    }

    return r;
}

template<class T, class S, class M, class X, class E>
bool tgbg_combinator<T, S, M, X, E>::could_be_final(const entry_type& entry)
{
    // odrzucamy symbole pomocnicze
    std::string symbol = symbol_reg_.get_obj(entry.get_cat());
    if (symbol.length() > 0
       && (symbol[0] == '<'
       || symbol[0] == '\'' && symbol.find(' ') != string::npos))
    return false;

    if (final_hook_expr_)
    {
    entry_type new_entry(final_hook_symbol_id_);

    typename expression_runner_type::runner_frame frame;
    frame.lhs = &new_entry;
    frame.rhs.push_back(&entry);

    if (!runner_.run(
           *final_hook_expr_,
           frame,
           master_,
           semantics_machine_))
        return false;
    }

    return true;
}

template<class T, class S, class M, class X, class E>
bool tgbg_combinator<T, S, M, X, E>::could_be_final_variant(
    const entry_type& entry, const variant_type& variant)
{
    if (final_hook_extra_expr_)
    {
    entry_type new_entry(final_hook_symbol_id_);
    variant_type new_variant_entry;

    typename expression_runner_type::runner_frame frame;
    frame.lhs = &new_entry;
    frame.rhs.push_back(&entry);
    frame.extra_lhs = &new_variant_entry;
    frame.extra_rhs.push_back(&variant);

    if (!runner_.run(
           *final_hook_extra_expr_,
           frame,
           master_,
           semantics_machine_))
        return false;
    }

    return true;
}

template<class T, class S, class M, class X, class E>
bool tgbg_combinator<T, S, M, X, E>::is_lexical(const rule_type& rule) const
{
    return rule.rule_ix_ < 0;
}

template<class T, class S, class M, class X, class E>
boost::shared_ptr<tree_specification<T> > tgbg_combinator<T, S, M, X, E>::tree_spec(
    const rule_type& rule, const std::vector<rule_holder>& local_rules)
{
    assert(rule.rule_ix_ < (signed int)rules_.size());

    if (rule.rule_ix_ < 0)
    {
    int local_rule_ix = -rule.rule_ix_ - 1;

    assert(local_rule_ix >= 0);

    if (local_rule_ix >= (signed int)local_rules.size())
        return empty_tree_spec_;

    assert(!local_rules[local_rule_ix].tree_specs.empty());

    return local_rules[local_rule_ix].tree_specs[0];
    }

    assert(rule.tree_choice_ < rules_[rule.rule_ix_].tree_specs.size());
    return rules_[rule.rule_ix_].tree_specs[rule.tree_choice_];
}

template<class T, class S, class M, class X, class E>
E tgbg_combinator<T, S, M, X, E>::equivalent(
    const rule_type& rule, const std::vector<rule_holder>& local_rules)
{
    assert(rule.rule_ix_ < (signed int)rules_.size());

    if (rule.rule_ix_ < 0)
    {
    int local_rule_ix = -rule.rule_ix_ - 1;

    assert(local_rule_ix >= 0);

    if (local_rule_ix >= (signed int)local_rules.size())
        return E();

    assert(!local_rules[local_rule_ix].tree_specs.empty());

    return local_rules[local_rule_ix].equivalent;
    }

    return rules_[rule.rule_ix_].equivalent;
}

template<class T, class S, class M, class X, class E>
bool tgbg_combinator<T, S, M, X, E>::is_useful(const entry_type& entry) const
{
    return entry.get_cat() <= last_useful_symbol_id_;
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::clear_parser_caches()
{
    runner_.clear();
}

template<class T, class S, class M, class X, class E>
bool tgbg_combinator<T, S, M, X, E>::variant_combine_unary(
    entry_type& super_entry,
    const entry_type& sub_entry,
    const variant_type& sub_variant,
    const rule_type& rule,
    variant_type& new_variant,
    S& score)
{
    const rule_holder& rh = rules_[rule.rule_ix_];

    if (rh.starred_ix == 0)
    new_variant = sub_variant;

    if (!rh.compiled_extra_expr)
    {
    score = 0;
    return true;
    }

    typename expression_runner_type::runner_frame frame;
    frame.lhs = &super_entry;
    frame.rhs.push_back(&sub_entry);
    frame.extra_lhs = &new_variant;
    frame.extra_rhs.push_back(&sub_variant);

    if (runner_.run(
       *(rh.compiled_extra_expr),
       frame,
       master_,
       semantics_machine_))
    {
    score = frame.score;
    return true;
    }

    return false;
}

template<class T, class S, class M, class X, class E>
bool tgbg_combinator<T, S, M, X, E>::variant_combine_binary(
    entry_type& super_entry,
    const entry_type& left_sub_entry,
    const entry_type& right_sub_entry,
    const variant_type& left_sub_variant,
    const variant_type& right_sub_variant,
    const rule_type& rule,
    variant_type& new_variant,
    S& score)
{
    const rule_holder& rh = rules_[rule.rule_ix_];

    if (rh.starred_ix == 0)
    new_variant = left_sub_variant;
    else if (rh.starred_ix == 1)
    new_variant = right_sub_variant;

    if (!rh.compiled_extra_expr)
    {
    score = 0;
    return true;
    }

    typename expression_runner_type::runner_frame frame;
    frame.lhs = &super_entry;
    frame.rhs.push_back(&left_sub_entry);
    frame.rhs.push_back(&right_sub_entry);
    frame.extra_lhs = &new_variant;
    frame.extra_rhs.push_back(&left_sub_variant);
    frame.extra_rhs.push_back(&right_sub_variant);

    if (runner_.run(
       *(rh.compiled_extra_expr),
       frame,
       master_,
       semantics_machine_))
    {
    score = frame.score;
    return true;
    }

    return false;
}

template<class T, class S, class M, class X, class E>
bool tgbg_combinator<T, S, M, X, E>::is_variant_independent(
    const rule_type& rule,
    int symbol_ix)
{
    assert(rule.rule_ix_ >= 0);

    const rule_holder& rh = rules_[rule.rule_ix_];

    assert(symbol_ix >= 0 && symbol_ix < (signed int)rh.extra_independence.size());
    assert(rh.rhs_symbols.size() == rh.extra_independence.size());

    return rh.extra_independence[symbol_ix];
}


template<class T, class S, class M, class X, class E>
S tgbg_combinator<T, S, M, X, E>::get_preference(T syncat)
{
    if (preference_table_.count(syncat))
    return preference_table_[syncat];

    return 0;
}

template<class T, class S, class M, class X, class E>
std::string tgbg_combinator<T, S, M, X, E>::get_option(const std::string& option_name) const
{
    if (options_.count(option_name))
    return (*options_.find(option_name)).second;

    return std::string();
}

template<class T, class S, class M, class X, class E>
size_t tgbg_combinator<T, S, M, X, E>::get_nb_limits() const
{
    return limits_.size();
}

template<class T, class S, class M, class X, class E>
int tgbg_combinator<T, S, M, X, E>::get_limit_threshold(size_t limit_ix) const
{
    assert(limit_ix < limits_.size());

    return limits_[limit_ix].threshold;
}

template<class T, class S, class M, class X, class E>
S tgbg_combinator<T, S, M, X, E>::get_limit_min_score(size_t limit_ix) const
{
    assert(limit_ix < limits_.size());

    return limits_[limit_ix].min_score;
}

template<class T, class S, class M, class X, class E>
int tgbg_combinator<T, S, M, X, E>::get_limit_edges(int len) const
{
    return 1000/len;
    // switch (len) {
        // case 1:
            // return -1L;
        // case 2:
            // return 30;
        // case 3:
            // return 20;
        // default:
            // if (len <= 12)
                // return 12;
            // else
                // return 8;
    // }
}

template<class T, class S, class M, class X, class E>
std::vector<typename tgbg_combinator<T, S, M, X, E>::rule_type>
    tgbg_combinator<T, S, M, X, E>::combine_binary(
    const entry_type& first_entry,
    const entry_type& second_entry)
{
    std::vector<rule_type> r;

    int first_symbol_id = first_entry.get_cat();
    int second_symbold_id = second_entry.get_cat();

    assert(first_symbol_id >= 0 && second_symbold_id >= 0);

    if (first_symbol_id < nb_indexed_symbols_ &&
       second_symbold_id < nb_indexed_symbols_)
    {
    std::vector<int>* matrix_cell = binary_rules_index_[
        first_symbol_id * nb_indexed_symbols_ + second_symbold_id];

    if (matrix_cell)
    {
        std::vector<int>::iterator end_iter = matrix_cell->end();
        for (std::vector<int>::iterator iter = matrix_cell->begin();
        iter != end_iter;
        ++iter)
        {
        int rule_ix = (*iter);

        compile_rule_(rule_ix);

        entry_type new_entry(rules_[rule_ix].lhs_symbol);;

        if (rules_[rule_ix].starred_ix == 0)
            new_entry.copy_attrs(first_entry);
        else if (rules_[rule_ix].starred_ix == 1)
            new_entry.copy_attrs(second_entry);

        typename expression_runner_type::runner_frame frame;
        frame.lhs = &new_entry;
        frame.rhs.reserve(2);
        frame.rhs.push_back(&first_entry);
        frame.rhs.push_back(&second_entry);

        if (runner_.run(
               *(rules_[rule_ix].compiled_expr),
               frame,
               master_,
               semantics_machine_))
                {
#ifdef _T5_WITH_EXTSCORES
                    try_using_extscore_(rules_[rule_ix], frame);
#endif
            r.push_back(rule_type(rule_ix, frame.chosen_tree_ix, frame.score, new_entry));
                }
        }
    }
    }

    return r;
}

template<class T, class S, class M, class X, class E>
bool tgbg_combinator<T, S, M, X, E>::are_rules_for_catprints(int catprint_a, int catprint_b) const
{
    return binary_rules_index_[catprint_a * nb_indexed_symbols_ + catprint_b] != 0;
}

template<class T, class S, class M, class X, class E>
registrar<std::string>& tgbg_combinator<T, S, M, X, E>::get_attribute_registrar()
{
    return attribute_reg_;
}

template<class T, class S, class M, class X, class E>
registrar<std::string>& tgbg_combinator<T, S, M, X, E>::get_extra_attribute_registrar()
{
    return extra_attribute_reg_;
}

template<class T, class S, class M, class X, class E>
registrar<std::string>& tgbg_combinator<T, S, M, X, E>::get_symbol_registrar()
{
    return symbol_reg_;
}

template<class T, class S, class M, class X, class E>
M& tgbg_combinator<T, S, M, X, E>::get_master()
{
    return master_;
}

template<class T, class S, class M, class X, class E>
X& tgbg_combinator<T, S, M, X, E>::get_semantics_machine()
{
    return semantics_machine_;
}

template<class T, class S, class M, class X, class E>
bool tgbg_combinator<T, S, M, X, E>::check_rules()
{
    for (size_t i = 0; i < rules_.size(); ++i)
    {
    const rule_holder& h = rules_[i];

    if (find_loops(*h.compiled_expr))
    {
        std::cerr << "loop in expression:" << std::endl;
        disassemble_expression(
        *h.compiled_expr, attribute_reg_, extra_attribute_reg_, master_, std::cerr);

        return false;
    }

    if (find_loops(*h.compiled_expr))
    {
        std::cerr << "extra ref in expression:" << std::endl;
        disassemble_expression(
        *h.compiled_expr, attribute_reg_, extra_attribute_reg_, master_, std::cerr);

        return false;
    }

    if (find_badindexes(*h.compiled_expr, h.rhs_symbols.size()))
    {
        std::cerr << "bad index in expression:" << std::endl;
        disassemble_expression(
        *h.compiled_expr, attribute_reg_, extra_attribute_reg_, master_, std::cerr);

        return false;
    }


    if (h.compiled_extra_expr)
    {
        if (find_loops(*h.compiled_extra_expr))
        {
        std::cerr << "loop in expression:" << std::endl;
        disassemble_expression(
            *h.compiled_extra_expr, attribute_reg_, extra_attribute_reg_, master_, std::cerr);

        return false;
        }

        if (find_loops(*h.compiled_extra_expr))
        {
        std::cerr << "extra ref in expression:" << std::endl;
        disassemble_expression(
            *h.compiled_extra_expr, attribute_reg_, extra_attribute_reg_, master_, std::cerr);

        return false;
        }

        if (find_badindexes(*h.compiled_expr, h.rhs_symbols.size()))
        {
        std::cerr << "bad index in expression:" << std::endl;
        disassemble_expression(
            *h.compiled_extra_expr,
            attribute_reg_, extra_attribute_reg_, master_, std::cerr);

        return false;
        }
    }

    }

    return true;
}


template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::print_rules(std::ostream& ostr, bool with_tree_specifications) const
{
    for (size_t i = 0; i < rules_.size(); ++i)
    {
    print_rule_(ostr, i, with_tree_specifications);
    ostr << std::endl << std::endl;
    }
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::print_rule_(
    std::ostream& ostr,
    size_t rule_ix,
    bool with_tree_specifications) const
{
    assert(rule_ix < rules_.size());
    const rule_holder& h = rules_[rule_ix];

    ostr << ( h.is_an_origin ? '*' : '+') << ' '
     << symbol_reg_.get_obj(h.lhs_symbol) << " =";

    for (int i = 0; i < (signed int)h.rhs_symbols.size(); ++i)
    {
    ostr << ' ' << symbol_reg_.get_obj(h.rhs_symbols[i]);
    if (i == h.starred_ix)
        ostr << '*';
    }

    if (with_tree_specifications)
    {
    ostr << "  ";

    if (h.tree_specs.empty())
        ostr << " %(?NONE?)%";
    else
    {
        for (typename std::vector<boost::shared_ptr<tree_specification<atom_type> > >::const_iterator
            ts_it
            = h.tree_specs.begin();
        ts_it != h.tree_specs.end();
        ++ts_it)
        {
        ostr << " %";

        if (*ts_it)
            ostr << (*ts_it)->string_representation(master_);
        else
            ostr << "(?NONE?)";

        ostr << '%';
        }
    }
    }

    ostr << std::endl;

    if (h.compiled_expr)
    disassemble_expression(
        *h.compiled_expr, attribute_reg_, extra_attribute_reg_, master_, ostr);

    if (h.compiled_extra_expr)
    {
    ostr << ">>\n";

    disassemble_expression(
        *h.compiled_extra_expr, attribute_reg_, extra_attribute_reg_, master_, ostr);
    }
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::add_rule_(GRule* g_rule)
{
    assert(g_rule);

    rules_.push_back(typename tgbg_combinator::rule_holder());
    rules_.back().g_rule.reset(g_rule);

    assert(g_rule->left_symbol);

    rules_.back().lhs_symbol = symbol_reg_.get_id(*(g_rule->left_symbol));

    int rule_ix = rules_.size() - 1;

    linearize_rule_(rule_ix);
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::index_rule_(int rule_ix)
{
    assert(rule_ix >= 0 && rule_ix < (signed int)rules_.size());

    if (!rules_[rule_ix].is_an_origin && !rules_[rule_ix].is_indexed)
    {
    assert(
        rules_[rule_ix].rhs_symbols.size() == 1 ||
        rules_[rule_ix].rhs_symbols.size() == 2);

    if (rules_[rule_ix].rhs_symbols.size() == 2)
        index_binary_rule_(rule_ix);
    else
        index_unary_rule_(rule_ix);

    apply_hooks_(rule_ix);

    rules_[rule_ix].is_indexed = true;

    if (rules_[rule_ix].compiled_expr)
    {
        rules_[rule_ix].extra_independence = get_independence_vector<atom_type,S>(
        rules_[rule_ix].compiled_extra_expr,
        rules_[rule_ix].rhs_symbols.size(),
        rules_[rule_ix].starred_ix);
    }
    }
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::index_unary_rule_(int rule_ix)
{
    rule_holder& rh = rules_[rule_ix];

    if (rh.rhs_symbols[0] >= (signed int)unary_rules_index_.size())
    unary_rules_index_.resize((3*(rh.rhs_symbols[0]+1))/2+2);

    unary_rules_index_[rh.rhs_symbols[0]].push_back(rule_ix);
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::index_binary_rule_(int rule_ix)
{
    rule_holder& rh = rules_[rule_ix];

    int symbol_a = rh.rhs_symbols[0];
    int symbol_b = rh.rhs_symbols[1];

    if (symbol_a >= nb_indexed_symbols_ ||
       symbol_b >= nb_indexed_symbols_)
    resize_binary_rules_index_((symbol_a >= symbol_b ? symbol_a : symbol_b));

    std::vector<int>*& index_entry =
    binary_rules_index_[symbol_a*nb_indexed_symbols_ + symbol_b];

    if (!index_entry)
    index_entry = new std::vector<int>();

    index_entry->push_back(rule_ix);
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::resize_binary_rules_index_(int sid)
{
    int new_nb_indexed_symbols = (3*(sid+1))/2+2;

    assert(new_nb_indexed_symbols > nb_indexed_symbols_);

    binary_rules_index_.resize(new_nb_indexed_symbols * new_nb_indexed_symbols);

    for (int i = nb_indexed_symbols_-1; i > 0; --i)
    for (int j = nb_indexed_symbols_-1; j >= 0; --j)
        binary_rules_index_[i*new_nb_indexed_symbols+j] =
        binary_rules_index_[i*nb_indexed_symbols_+j];
    for (int i = 0; i < new_nb_indexed_symbols; ++i)
    for (int j = 0; j < new_nb_indexed_symbols; ++j)
        if (i >= nb_indexed_symbols_ ||
           j >= nb_indexed_symbols_)
        binary_rules_index_[i*new_nb_indexed_symbols+j] = 0;

    nb_indexed_symbols_ = new_nb_indexed_symbols;
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::check_special_rule_(int rule_ix)
{
    assert(pre_hook_symbol_id_ >= 0);

    rule_holder& rh = rules_[rule_ix];

    if (rh.rhs_symbols.size() == 2
       && rh.rhs_symbols[0] == pre_hook_symbol_id_
       && rh.rhs_symbols[1] == pre_hook_symbol_id_)
    {
    compile_rule_(rule_ix);

    pre_hook_binary_expr_.reset(
        new compiled_expression<atom_type, S, 2>(
        *rh.compiled_expr));

    if (pre_hook_binary_expr_)
        pre_hook_binary_expr_->pop_if_instruction(gobio_opcodes::OPCODE_STOP);

    rh.is_an_origin = true;
    }

    if (rh.rhs_symbols.size() == 1)
    {
    if (rh.rhs_symbols[0] == pre_hook_symbol_id_)
    {
        compile_rule_(rule_ix);

        pre_hook_unary_expr_.reset(
        new compiled_expression<atom_type, S, 2>(
            *rh.compiled_expr));

        if (pre_hook_unary_expr_)
        pre_hook_unary_expr_->pop_if_instruction(gobio_opcodes::OPCODE_STOP);

        rh.is_an_origin = true;
    }
        else if (rh.rhs_symbols[0] == final_hook_symbol_id_)
    {
        compile_rule_(rule_ix);

        final_hook_expr_.reset(
        new compiled_expression<atom_type, S, 2>(
            *rh.compiled_expr));

        if (rh.compiled_extra_expr)
        final_hook_extra_expr_.reset(
            new compiled_expression<atom_type, S, 2>(
            *rh.compiled_extra_expr));

        rh.is_an_origin = true;
    }
        else if (rh.rhs_symbols[0] == extra_attributes_symbol_id_)
    {
        compile_rule_(rule_ix);

        boost::shared_ptr<compiled_expression<atom_type, S, 2> > ce
        = rules_[rule_ix].compiled_expr;

        assert(ce);

        for (int i = 0; i < ce->first_available_address(); ++i)
        {
        if (ce->get_opcode(i) == gobio_opcodes::OPCODE_PUSH
           || ce->get_opcode(i) == gobio_opcodes::OPCODE_SETTOP)
        {
            atom_type v = ce->get_targ(i, 0);

            if (!master_.is_string(v))
            throw std::runtime_error("only strings expected in __EXTRA_ATTRIBUTES__");

            extra_attribute_reg_.get_id(
            master_.to_string(v));
        }
        }

        rh.is_an_origin = true;
    }
        else if (rh.rhs_symbols[0] == setscore_factor_id_)
    {
        setscore_factor_ = 1;

        read_score_encoded_in_special_rule_(rule_ix, setscore_factor_);
    }
        else if (rh.rhs_symbols[0] == preference_id_)
    {
        compile_rule_(rule_ix);

        boost::shared_ptr<compiled_expression<atom_type, S, 2> > ce
        = rules_[rule_ix].compiled_expr;

        assert(ce);

        atom_type fvalue = master_.false_value();
        atom_type current_value = fvalue;

        for (int i = 0; i < ce->first_available_address(); ++i)
        {
        if (ce->get_opcode(i) == gobio_opcodes::OPCODE_PUSH
           || ce->get_opcode(i) == gobio_opcodes::OPCODE_SETTOP)
            current_value = ce->get_targ(i, 0);

        if (ce->get_opcode(i) == gobio_opcodes::OPCODE_SET_SCORE
           && current_value != fvalue)
            preference_table_[current_value] = ce->get_farg(i, 0);
        }

        rh.is_an_origin = true;
    }
        else if (rh.rhs_symbols[0] == options_id_)
    {
        compile_rule_(rule_ix);

        boost::shared_ptr<compiled_expression<atom_type, S, 2> > ce
        = rules_[rule_ix].compiled_expr;

        assert(ce);

        atom_type options_name = master_.false_value();

        for (int i = 0; i < ce->first_available_address(); ++i)
        {
        if (ce->get_opcode(i) == gobio_opcodes::OPCODE_PUSH
           || ce->get_opcode(i) == gobio_opcodes::OPCODE_SETTOP)
            options_name = ce->get_targ(i, 0);

        if (ce->get_opcode(i) == gobio_opcodes::OPCODE_EQUAL_CONST)
            options_[master_.string_representation(options_name)]
            = master_.string_representation(
                ce->get_targ(i, 0));
        }

        rh.is_an_origin = true;
    }
        else if (rh.rhs_symbols[0] == limits_id_)
    {
        compile_rule_(rule_ix);

        boost::shared_ptr<compiled_expression<atom_type, S, 2> > ce
        = rules_[rule_ix].compiled_expr;

        assert(ce);

        int th = -1L;

        limits_.clear();

        for (int i = 0; i < ce->first_available_address(); ++i)
        {
        if ((ce->get_opcode(i) == gobio_opcodes::OPCODE_PUSH
            || ce->get_opcode(i) == gobio_opcodes::OPCODE_SETTOP)
           && master_.is_string(ce->get_targ(i, 0)))
            th = atoi(master_.to_string(ce->get_targ(i, 0)).c_str());

        if (ce->get_opcode(i) == gobio_opcodes::OPCODE_SET_SCORE
           && th >= 0)
        {
            limits_.push_back(limits_entry());
            limits_.back().threshold = th;
            limits_.back().min_score = ce->get_farg(i, 0);
            th = -1L;
        }
        }

        assert(limits_.size() > 0);

        rh.is_an_origin = true;
    }
    }
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::read_score_encoded_in_special_rule_(int rule_ix, S& encoded_value)
{
    rule_holder& rh = rules_[rule_ix];

    compile_rule_(rule_ix);

    boost::shared_ptr<compiled_expression<atom_type, S, 2> > ce
    = rules_[rule_ix].compiled_expr;

    assert(ce);

    for (int i = 0; i < ce->first_available_address(); ++i)
    {
    if (ce->get_opcode(i) == gobio_opcodes::OPCODE_SET_SCORE)
        encoded_value = ce->get_farg(i, 0);
    }

    rh.is_an_origin = true;
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::apply_hooks_(int rule_ix)
{
    rule_holder & rh = rules_[rule_ix];

    if (rh.compiled_expr && !rh.is_prehooked)
    {
    if (pre_hook_binary_expr_ && rh.rhs_symbols.size() == 2)
    {
        boost::shared_ptr<compiled_expression<atom_type, S, 2> > t = rh.compiled_expr;

        int delta = pre_hook_binary_expr_->first_available_address();
        rh.compiled_expr.reset(new compiled_expression<atom_type, S, 2>(*pre_hook_binary_expr_));
        rh.compiled_expr->append(*t);
        relocate_jumps(*rh.compiled_expr, delta, delta);
        rh.is_prehooked = true;
    }
    else if (pre_hook_unary_expr_ && rh.rhs_symbols.size() == 1)
    {
        boost::shared_ptr<compiled_expression<atom_type, S, 2> > t = rh.compiled_expr;

        int delta = pre_hook_unary_expr_->first_available_address();
        rh.compiled_expr.reset(new compiled_expression<atom_type, S, 2>(*pre_hook_unary_expr_));
        rh.compiled_expr->append(*t);
        relocate_jumps(*rh.compiled_expr, delta, delta);
        rh.is_prehooked = true;
    }
    }
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::linearize_rule_(int rule_ix)
{
    assert(rule_ix >= 0 && rule_ix < (signed int)rules_.size());

    if (rules_[rule_ix].is_an_origin)
    return;

    boost::shared_ptr<GRule> g_rule = rules_[rule_ix].g_rule;

    // sprawdzamy, czy nie jest już regułą liniową
    assert(g_rule->right_symbols);

    rule_holder& h = rules_[rule_ix];

    for (int i = 0; i < (signed int)g_rule->right_symbols->size(); ++i)
    {
    GRuleRightSymbolAltsSeq* alt = g_rule->right_symbols->at(i);

    assert(alt);

    if (alt->atom && !alt->is_optional)
    {
        h.rhs_symbols.push_back(symbol_reg_.get_id(*(alt->atom)));

        if (alt->is_starred)
        h.starred_ix = i;
    }
    else
    {
        h.is_an_origin = true;
        break;
    }
    }

    if (h.is_an_origin)
    {
    compile_rule_(rule_ix);

    std::vector<typename tgbg_combinator::rule_holder> v(1);
    v.back().lhs_symbol = rules_[rule_ix].lhs_symbol;
    v.back().origin = rule_ix;
    v.back().compiled_expr = rules_[rule_ix].compiled_expr;
    v.back().compiled_extra_expr = rules_[rule_ix].compiled_extra_expr;
    v.back().starred_ix = rules_[rule_ix].starred_ix;
    v.back().tree_specs = rules_[rule_ix].tree_specs;

    v = linearize_seq_(v, g_rule->right_symbols);

    for (typename std::vector<rule_holder>::const_iterator
        it = v.begin();
        it != v.end();
        ++it)
    {
        // wrzucamy tylko niepuste reguły
        if ((*it).rhs_symbols.size() > 0)
        rules_.push_back((*it));
    }
    }
}

template<class T, class S, class M, class X, class E>
std::vector<typename tgbg_combinator<T, S, M, X, E>::rule_holder>
          tgbg_combinator<T, S, M, X, E>::linearize_seq_(
    const std::vector<rule_holder>& v,
    const GRuleRightSymbolsSeq* seq)
{
    assert(seq);

    std::vector<typename tgbg_combinator::rule_holder> w(v);

    for (size_t i = 0; i < seq->size(); ++i)
    w = linearize_alt_(w,(*seq)[i]);

    return w;
}

template<class T, class S, class M, class X, class E>
std::vector<typename tgbg_combinator<T, S, M, X, E>::rule_holder>
          tgbg_combinator<T, S, M, X, E>::linearize_alt_(
    const std::vector<rule_holder>& v,
    const GRuleRightSymbolAltsSeq* alt)
{
    assert(alt);

    std::vector<typename tgbg_combinator::rule_holder> u;

    const size_t m = grule_alt_length(alt);

    if (alt->is_optional)
    {
    for (typename std::vector<typename tgbg_combinator::rule_holder>::const_iterator it
        = v.begin();
        it != v.end();
        ++it)
    {
        typename tgbg_combinator::rule_holder rh((*it));

        relocate_grule_(
        rh,
        (signed int)rh.rhs_symbols.size(),
        (signed int)rh.rhs_symbols.size() + m);

        u.push_back(rh);
    }
    }

    if (alt->atom)
    {
    for (typename std::vector<typename tgbg_combinator::rule_holder>::const_iterator
        it = v.begin();
        it != v.end();
        ++it)
    {
        typename tgbg_combinator::rule_holder rh((*it));

        rh.rhs_symbols.push_back(symbol_reg_.get_id(*(alt->atom)));

        u.push_back(rh);
    }
    }
    else
    {
    for (size_t i = 0; i < alt->alts.size(); ++i)
    {
        std::vector<typename tgbg_combinator::rule_holder> t
        = linearize_seq_(v,(alt->alts)[i]);

        const int sub_m = grule_seq_length((alt->alts)[i]);

        for (typename std::vector<typename tgbg_combinator::rule_holder>::const_iterator
            it = t.begin();
        it != t.end();
        ++it)
        {
        typename tgbg_combinator::rule_holder rh((*it));

        relocate_grule_(rh, sub_m, m);

        u.push_back(rh);
        }
    }
    }

    return u;
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::decompose_rule_(int rule_ix)
{
    if (rules_[rule_ix].compiled_expr)
    {
    std::vector<boost::shared_ptr<compiled_expression<atom_type, S, 2> > > parts;

    boost::shared_ptr<compiled_expression<atom_type, S, 2> > new_expr = decompose_expression(
        rules_[rule_ix].compiled_expr,
        rules_[rule_ix].rhs_symbols.size(),
        rules_[rule_ix].starred_ix,
        parts);

    assert(parts.size() == rules_[rule_ix].rhs_symbols.size());

    bool was_changed = false;
    std::vector<int> rhs_smbs = rules_[rule_ix].rhs_symbols;

    for (size_t p = 0; p < parts.size(); ++p)
    {
        boost::shared_ptr<compiled_expression<atom_type, S, 2> > expr
        = parts[p];

        if (expr)
        {
        was_changed = true;

        if (p != 0)
            relocate_attributes_from_symbol_to_symbol(
            *expr,
            p,
            0,
            gobio_opcodes::OPCODE_PUSH_SUBVAR,
            gobio_opcodes::OPCODE_SETTOP_SUBVAR);

        if (!filtres_hash_.count(
               std::pair<int, compiled_expression<atom_type, S, 2> >(rhs_smbs[p],*expr)))
        {
            rules_.push_back(rule_holder());
            rules_.back().compiled_expr = parts[p];
            rules_.back().extra_independence.resize(1);
            rules_.back().extra_independence[0] = true;
            rules_.back().tree_specs.push_back(
            default_tree_spec<atom_type>(1, 0, master_));
            rules_.back().lhs_symbol = get_filtre_id_(
            rhs_smbs[p],
            last_filtre_ix_++);
            rules_.back().rhs_symbols.resize(1);
            rules_.back().rhs_symbols[0]
            = rhs_smbs[p];
            rules_.back().starred_ix = 0;

            filtres_hash_[std::pair<int, compiled_expression<atom_type, S, 2> >
                  (rhs_smbs[p],*expr)]
            = rules_.back().lhs_symbol;
        }

        rhs_smbs[p] = filtres_hash_[
            std::pair<int, compiled_expression<atom_type, S, 2> >
            (rhs_smbs[p],*expr)];
        }
    }

    if (was_changed)
    {
        rules_.push_back(rules_[rule_ix]);

        rules_.back().origin = rule_ix;
        rules_.back().g_rule.reset();
        rules_.back().rhs_symbols = rhs_smbs;
        rules_.back().compiled_expr = new_expr;

        rules_[rule_ix].is_an_origin = true;
    }
    }
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::relocate_grule_(
    rule_holder& rh, int begin_attr_ix, int end_attr_ix, bool delete_star)
{
    rh.compiled_expr.reset(new compiled_expression<atom_type, S, 2>(*rh.compiled_expr));
    relocate_expression(*rh.compiled_expr, begin_attr_ix, end_attr_ix);

    if (rh.compiled_extra_expr)
    {
    rh.compiled_extra_expr.reset(
        new compiled_expression<atom_type, S, 2>(*rh.compiled_extra_expr));
    relocate_expression(*rh.compiled_extra_expr, begin_attr_ix, end_attr_ix);
    }


    if (delete_star && rh.starred_ix >= begin_attr_ix
       && rh.starred_ix < end_attr_ix)
    rh.starred_ix = -1L;
    else if (rh.starred_ix >= end_attr_ix)
    rh.starred_ix -= (end_attr_ix - begin_attr_ix);

    for (typename std::vector<boost::shared_ptr<tree_specification<atom_type> > >::iterator ts_it
        = rh.tree_specs.begin();
    ts_it != rh.tree_specs.end();
    ++ts_it)
    {
    (*ts_it) = (*ts_it)->clone();
    if (delete_star)
        (*ts_it)->remove_hooks(begin_attr_ix, end_attr_ix);
    (*ts_it)->relocate(end_attr_ix, -(end_attr_ix - begin_attr_ix));
    }
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::compile_rule_(int rule_ix)
{
    assert(rule_ix < (signed int)rules_.size());

    rule_holder& rh = rules_[rule_ix];

    if (!rh.compiled_expr || rh.tree_specs.empty())
    {
    typename HashWrapper<std::pair<int, int>, int>::type rhs_hash;

    int current_ix = 0;
    int starred_ix = -1L;

    assert(rh.g_rule);
    assert(rh.g_rule->right_symbols);

    right_symbols_to_hash(
        *(rh.g_rule->right_symbols),
        rhs_hash,
        symbol_reg_,
        current_ix,
        starred_ix);

    if (rh.tree_specs.empty())
    {
        assert(rh.g_rule);

        rh.tree_specs.push_back(
        ((rh.g_rule->recipe)
         ? convert_tree_recipe<atom_type,M>(
             rh.g_rule->recipe,
             rhs_hash,
             master_,
             symbol_reg_,
             starred_ix)
         : default_tree_spec<atom_type>(current_ix, starred_ix, master_, true)));
    }

    if (!rh.compiled_expr)
    {
        assert(rh.g_rule);

        rh.compiled_expr.reset(new compiled_expression<atom_type, S, 2>());

        assert(starred_ix >= -1L);
        rh.starred_ix = starred_ix;

        compile_gruleexpression(
        rh.g_rule->expr,
        rhs_hash,
        *(rh.compiled_expr),
        master_,
        attribute_reg_,
        extra_attribute_reg_,
        symbol_reg_,
        starred_ix,
        rh.tree_specs,
        setscore_factor_);

        if (rh.is_indexed)
        apply_hooks_(rule_ix);

        assert(!find_loops(*(rh.compiled_expr)));
        assert(!find_extras(*(rh.compiled_expr)));

        if (rh.g_rule->extra_expr)
        {
        assert(!rh.compiled_extra_expr);

        rh.compiled_extra_expr.reset(new compiled_expression<atom_type, S, 2>());

        std::vector<boost::shared_ptr<tree_specification<atom_type> > > tree_specs_stub;
        tree_specs_stub.push_back(boost::shared_ptr<tree_specification<atom_type> >());

        compile_gruleexpression(
            rh.g_rule->extra_expr,
            rhs_hash,
            *(rh.compiled_extra_expr),
            master_,
            attribute_reg_,
            extra_attribute_reg_,
            symbol_reg_,
            starred_ix,
            tree_specs_stub);

        assert(tree_specs_stub.size() == 1);

        assert(!find_loops(*(rh.compiled_extra_expr)));
        assert(!find_non_extras_assignments(*(rh.compiled_extra_expr)));
        }

        if (rh.is_indexed)
        rh.extra_independence = get_independence_vector<atom_type,S>(
            rh.compiled_extra_expr,
            rh.rhs_symbols.size(),
            starred_ix);
    }
    }
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::gather_bids_(
    typename list<rule_holder>::iterator rule_it,
    typename HashWrapper<
    bid_key,
    bid_info>::type& bids)
{
    assert((*rule_it).rhs_symbols.size() > 2);

    for (int symbol_ix = 0;
    symbol_ix < (signed int)((*rule_it).rhs_symbols.size() - 1); ++symbol_ix)
    for (int d = 0; d <= 1; ++d)
        add_bid_(bids, rule_it, symbol_ix, d);
}

template<class T, class S, class M, class X, class E>
int tgbg_combinator<T, S, M, X, E>::get_filtre_id_(int symbol_ix, int filtre_ix)
{
    std::ostringstream ostr;
    ostr << filtre_ix;

    return symbol_reg_.get_id(
    std::string("<~")
    + symbol_reg_.get_obj(symbol_ix)
    + std::string("-")
    + ostr.str()
    + std::string(">"));
}

template<class T, class S, class M, class X, class E> template<class Archive>
void tgbg_combinator<T, S, M, X, E>::save(Archive & ar, const unsigned int /*version*/) const
{
    ar & attribute_reg_;
    ar & extra_attribute_reg_;
    ar & symbol_reg_;
    ar & master_;
    //ar & semantics_machine_;
    ar & rules_;

    ar & unary_rules_index_;
    ar & binary_rules_index_;
    ar & nb_indexed_symbols_;

    ar & pre_hook_symbol_id_;
    ar & pre_hook_binary_expr_;
    ar & pre_hook_unary_expr_;
    ar & final_hook_symbol_id_;
    ar & final_hook_expr_;
    ar & final_hook_extra_expr_;
    ar & extra_attributes_symbol_id_;
    ar & setscore_factor_id_;
    ar & setscore_factor_;
    ar & preference_id_;
    ar & preference_table_;

    ar & options_id_;
    ar & options_;
    ar & limits_id_;
    ar & limits_;

    //expression_runner_type runner_;
    ar & empty_tree_spec_;
    ar & last_useful_symbol_id_;
}

template<class T, class S, class M, class X, class E> template<class Archive>
void tgbg_combinator<T, S, M, X, E>::load(Archive & ar, const unsigned int /*version*/)
{
    ar & attribute_reg_;
    ar & extra_attribute_reg_;
    ar & symbol_reg_;
    ar & master_;
    //ar & semantics_machine_;
    ar & rules_;

    ar & unary_rules_index_;
    ar & binary_rules_index_;
    ar & nb_indexed_symbols_;

    ar & pre_hook_symbol_id_;
    ar & pre_hook_binary_expr_;
    ar & pre_hook_unary_expr_;
    ar & final_hook_symbol_id_;
    ar & final_hook_expr_;
    ar & final_hook_extra_expr_;
    ar & extra_attributes_symbol_id_;
    ar & setscore_factor_id_;
    ar & setscore_factor_;
    ar & preference_id_;
    ar & preference_table_;

    ar & options_id_;
    ar & options_;
    ar & limits_id_;
    ar & limits_;

    //expression_runner_type runner_;
    ar & empty_tree_spec_;
    ar & last_useful_symbol_id_;
}





// vvv METODY POTRZEBNE DO ASTAR vvv

/*
 * T - typ kategorii
 * zamiast T - entry_type
 * zamiast R jest rule_type
 *
 * rule_holder reprezentuje regule w sensie jak sie podaje te reguły
 * rule_type - jak oczekuje tego maszyneria
 * ma metody category() i score() i numerek do wektora rule_holderow "rules_"
 *  -> lhs_symbol, rhs_symbol sa w rule_holderze
 *
 * is_indexed mowi, czy regula zostala zaindeksowana (a zatem zbinaryzowana),
 * wiec uzyc tego
 */

template<class T, class S, class M, class X, class E>
std::vector<typename tgbg_combinator<T, S, M, X, E>::rule_type>
tgbg_combinator<T, S, M, X, E>::find_only_child_parent(const entry_type& symbol)
{
    return combine_unary(symbol);
}

template<class T, class S, class M, class X, class E>
std::vector< std::pair<typename tgbg_combinator<T, S, M, X, E>::rule_type,typename tgbg_combinator<T, S, M, X, E>::entry_type> > tgbg_combinator<T, S, M, X, E>::find_left_sibling_with_parent(typename tgbg_combinator<T, S, M, X, E>::entry_type second_entry)
{
    std::vector< std::pair< rule_type, entry_type > > r;
    int second_symbol_id = second_entry.get_cat();
    assert(second_symbol_id >= 0);
    for (
        typename std::vector< std::vector<int>* >::iterator brii1 = binary_rules_index_.begin();
        brii1 != binary_rules_index_.end();
        ++brii1
    ) {
        std::vector<int>* matrix_cell = *brii1;
        if (matrix_cell) {
            for (
                typename std::vector<int>::iterator brii2 = matrix_cell->begin();
                brii2 != matrix_cell->end();
                ++brii2
            ) {
                int rule_ix = (*brii2);
                if (rules_[rule_ix].is_indexed && (rules_[rule_ix].rhs_symbols[1] == second_symbol_id)) {
                    compile_rule_(rule_ix);
                    assert(rules_[rule_ix].compiled_expr);
                    entry_type parent_entry(rules_[rule_ix].lhs_symbol);
                    entry_type first_entry(rules_[rule_ix].rhs_symbols[0]);
                    typename expression_runner_type::runner_frame frame;
                    frame.lhs = &parent_entry;
                    frame.rhs.reserve(2);
                    frame.rhs.push_back(&first_entry);
                    frame.rhs.push_back(&second_entry);
                    if (runner_.run(
                        *(rules_[rule_ix].compiled_expr),
                        frame,
                        master_,
                        semantics_machine_
                    )) {
                        r.push_back(std::pair< rule_type, entry_type >(
                            rule_type(rule_ix, frame.chosen_tree_ix, frame.score, parent_entry),
                            first_entry
                        ));
                    }
                }
            }
        }
    }
    return r;
}

template<class T, class S, class M, class X, class E>
std::vector< std::pair<typename tgbg_combinator<T, S, M, X, E>::rule_type,typename tgbg_combinator<T, S, M, X, E>::entry_type> > tgbg_combinator<T, S, M, X, E>::find_right_sibling_with_parent(typename tgbg_combinator<T, S, M, X, E>::entry_type first_entry)
{
    std::vector< std::pair< rule_type, entry_type > > r;
    int first_symbol_id = first_entry.get_cat();
    assert(first_symbol_id >= 0);
    for (
        typename std::vector< std::vector<int>* >::iterator brii1 = binary_rules_index_.begin();
        brii1 != binary_rules_index_.end();
        ++brii1
    ) {
        std::vector<int>* matrix_cell = *brii1;
        if (matrix_cell) {
            for (
                typename std::vector<int>::iterator brii2 = matrix_cell->begin();
                brii2 != matrix_cell->end();
                ++brii2
            ) {
                int rule_ix = (*brii2);
                if (rules_[rule_ix].is_indexed && (rules_[rule_ix].rhs_symbols[0] == first_symbol_id)) {
                    compile_rule_(rule_ix);
                    assert(rules_[rule_ix].compiled_expr);
                    entry_type parent_entry(rules_[rule_ix].lhs_symbol);
                    entry_type second_entry(rules_[rule_ix].rhs_symbols[1]);
                    typename expression_runner_type::runner_frame frame;
                    frame.lhs = &parent_entry;
                    frame.rhs.reserve(2);
                    frame.rhs.push_back(&first_entry);
                    frame.rhs.push_back(&second_entry);
                    if (runner_.run(
                        *(rules_[rule_ix].compiled_expr),
                        frame,
                        master_,
                        semantics_machine_
                    )) {
                        r.push_back(std::pair< rule_type, entry_type >(
                            rule_type(rule_ix, frame.chosen_tree_ix, frame.score, parent_entry),
                            second_entry
                        ));
                    }
                }
            }
        }
    }
    return r;
}

template<class T, class S, class M, class X, class E>
std::vector< std::pair< typename tgbg_combinator<T, S, M, X, E>::rule_type, std::pair<typename tgbg_combinator<T, S, M, X, E>::entry_type,typename tgbg_combinator<T, S, M, X, E>::entry_type> > > tgbg_combinator<T, S, M, X, E>::split(typename tgbg_combinator<T, S, M, X, E>::entry_type parent_entry)
{
    std::vector< std::pair< rule_type, std::pair< entry_type, entry_type > > > r;
    int parent_symbol_id = parent_entry.get_cat();
    assert(parent_symbol_id >= 0);
    for (
        typename std::vector< std::vector<int>* >::iterator brii1 = binary_rules_index_.begin();
        brii1 != binary_rules_index_.end();
        ++brii1
    ) {
        std::vector<int>* matrix_cell = *brii1;
        if (matrix_cell) {
            for (
                typename std::vector<int>::iterator brii2 = matrix_cell->begin();
                brii2 != matrix_cell->end();
                ++brii2
            ) {
                int rule_ix = (*brii2);
                if (rules_[rule_ix].is_indexed && (rules_[rule_ix].lhs_symbol == parent_symbol_id)) {
                    compile_rule_(rule_ix);
                    assert(rules_[rule_ix].compiled_expr);
                    entry_type first_entry(rules_[rule_ix].rhs_symbols[0]);
                    entry_type second_entry(rules_[rule_ix].rhs_symbols[1]);
                    typename expression_runner_type::runner_frame frame;
                    frame.lhs = &parent_entry;
                    frame.rhs.reserve(2);
                    frame.rhs.push_back(&first_entry);
                    frame.rhs.push_back(&second_entry);
                    if (runner_.run(
                        *(rules_[rule_ix].compiled_expr),
                        frame,
                        master_,
                        semantics_machine_
                    )) {
                        r.push_back(std::pair< rule_type, std::pair< entry_type, entry_type > >(
                            rule_type(rule_ix, frame.chosen_tree_ix, frame.score, parent_entry),
                            std::pair< entry_type, entry_type >(first_entry, second_entry)
                        ));
                    }
                }
            }
        }
    }
    return r;
}

template<class T, class S, class M, class X, class E>
void tgbg_combinator<T, S, M, X, E>::set_root(typename tgbg_combinator<T, S, M, X, E>::entry_type state) {
    // W TGBG JEST NIEPOTRZEBNA. KORZEN BRANY Z PLIKU WEJSCIOWEGO.
}

template<class T, class S, class M, class X, class E>
bool tgbg_combinator<T, S, M, X, E>::is_root(typename tgbg_combinator<T, S, M, X, E>::entry_type state) {
/*
#if USE_ASTAR_IN_PARSER
    return (state.get_cat() == symbol_reg_.get_id("__ROOT__") || state.get_cat() == symbol_reg_.get_id("simpx"));
#else
*/
    return (state.get_cat() == symbol_reg_.get_id("__ROOT__"));
/*
#endif
*/
}



// ^^^ METODY POTRZEBNE DO ASTAR ^^^







#endif
