#ifndef AGENDA_PARSER_HPP_HDR
#define AGENDA_PARSER_HPP_HDR

// #include "chart.tpl"
// #include "marked_edges_index.tpl"

template<typename C, typename S, typename V, typename R, typename K, typename A,
     template<class,class> class I>
class apply_binary_rules_algorithm;

template<typename C, typename S, typename V, typename R, typename K, typename A>
class apply_binary_rules_algorithm<C,S,V,R,K,A,simple_marked_edges_index>
{
public:
    typedef chart<C,S,V,R,simple_marked_edges_index> chart_type;

    typedef typename chart_type::marked_edges_index_type::marked_in_edge_iterator marked_in_edge_iterator;
    typedef typename chart_type::marked_edges_index_type::marked_out_edge_iterator marked_out_edge_iterator;

    static void apply(
    typename chart_type::edge_descriptor e,
    S min_score,
    chart_type& chart_,
    K& combinator_,
    A& agenda_)
    {
        // zastosowanie regu³ binarnych z bie¿±c± krawêdzi± po prawej stronie
        typename chart_type::vertex_descriptor source = chart_.edge_source(e);

        std::pair<
        marked_in_edge_iterator,
        marked_in_edge_iterator> ins =
        chart_.marked_edges_index(source).marked_in_edges();

        for(marked_in_edge_iterator it = ins.first;
        it != ins.second;
        ++it)
        apply_binary_rules_for_pair_((*it), e, min_score, chart_, combinator_, agenda_);

        // zastosowanie regu³ binarnych z bie¿±c± krawêdzi± po lewej stronie
        typename chart_type::vertex_descriptor target = chart_.edge_target(e);

        std::pair<
        marked_out_edge_iterator,
        marked_out_edge_iterator> outs =
        chart_.marked_edges_index(target).marked_out_edges();

        for(marked_out_edge_iterator it = outs.first;
        it != outs.second;
        ++it)
        apply_binary_rules_for_pair_(e, (*it), min_score, chart_, combinator_, agenda_);
    }

private:
    static void apply_binary_rules_for_pair_(
    typename chart_type::edge_descriptor e,
    typename chart_type::edge_descriptor f,
    S min_score,
    chart_type& chart_,
    K& combinator_,
    A& agenda_)
    {
        if (chart_.edge_score(e) < min_score ||
        chart_.edge_score(f) < min_score)
        return;

        std::vector<R> rules(combinator_.combine_binary(
                     chart_.edge_category(e),
                     chart_.edge_category(f)));

        if(rules.empty())
        return;

        typename chart_type::vertex_descriptor source = chart_.edge_source(e);
        typename chart_type::vertex_descriptor target = chart_.edge_target(f);
        S escore = chart_.edge_score(e) + chart_.edge_score(f);

        for(typename std::vector<R>::iterator it = rules.begin();
        it != rules.end();
        ++it)
        {
        std::pair<typename chart_type::edge_descriptor, bool> p
            = chart_.add_edge(
            source,
            target,
            (*it).category(),
            escore + (*it).score(),
            (*it),
            e,
            f);

        if(p.second)
            agenda_.push(p.first);
        }
    }
};

template<typename C, typename S, typename V, typename R, typename K, typename A>
class apply_binary_rules_algorithm<C,S,V,R,K,A,layered_marked_edges_index>
{
public:
    typedef chart<C,S,V,R,layered_marked_edges_index> chart_type;
    typedef typename chart_type::marked_edges_index_type marked_edges_index_type;

    static void apply(
    typename chart_type::edge_descriptor e,
    S min_score,
    chart_type& chart_,
    K& combinator_,
    A& agenda_)
    {
        typename C::catprint_type cp = chart_.edge_category(e).catprint();

        {
        typename chart_type::vertex_descriptor source = chart_.edge_source(e);

        std::pair<
            typename marked_edges_index_type::catprint_iterator,
            typename marked_edges_index_type::catprint_iterator> ins =
            chart_.marked_edges_index(source).in_catprints();

        for(typename marked_edges_index_type::catprint_iterator it = ins.first;
            it != ins.second;
            ++it)
            if(combinator_.are_rules_for_catprints((*it).first, cp))
            {
            std::pair<
                typename marked_edges_index_type::edge_iterator,
                typename marked_edges_index_type::edge_iterator> eins =
                chart_.marked_edges_index(source).in_edges(it);

            for(typename marked_edges_index_type::edge_iterator et = eins.first;
                et != eins.second;
                ++et)
                apply_binary_rules_for_pair_(*et, e, min_score,
                             chart_, combinator_, agenda_);
            }
        }

        {
        typename chart_type::vertex_descriptor target = chart_.edge_target(e);

        std::pair<
            typename marked_edges_index_type::catprint_iterator,
            typename marked_edges_index_type::catprint_iterator> outs =
            chart_.marked_edges_index(target).out_catprints();

        for(typename marked_edges_index_type::catprint_iterator it = outs.first;
            it != outs.second;
            ++it)
            if(combinator_.are_rules_for_catprints(cp, (*it).first))
            {
            std::pair<
                typename marked_edges_index_type::edge_iterator,
                typename marked_edges_index_type::edge_iterator> eouts =
                chart_.marked_edges_index(target).out_edges(it);

            for(typename marked_edges_index_type::edge_iterator et = eouts.first;
                et != eouts.second;
                ++et)
                apply_binary_rules_for_pair_(e, *et, min_score,
                             chart_, combinator_, agenda_);
            }
        }
        }



private:
    static void apply_binary_rules_for_pair_(
    typename chart_type::edge_descriptor e,
    typename chart_type::edge_descriptor f,
    S min_score,
    chart_type& chart_,
    K& combinator_,
    A& agenda_)
    {
        if (chart_.edge_score(e) < min_score ||
        chart_.edge_score(f) < min_score)
        return;

        std::vector<R> rules(combinator_.combine_binary(
                     chart_.edge_category(e),
                     chart_.edge_category(f)));

        if(rules.empty())
        return;

        typename chart_type::vertex_descriptor source = chart_.edge_source(e);
        typename chart_type::vertex_descriptor target = chart_.edge_target(f);
        S escore = chart_.edge_score(e) + chart_.edge_score(f);

        for(typename std::vector<R>::iterator it = rules.begin();
        it != rules.end();
        ++it)
        {
        std::pair<typename chart_type::edge_descriptor, bool> p
            = chart_.add_edge(
            source,
            target,
            (*it).category(),
            escore + (*it).score(),
            (*it),
            e,
            f);

        if(p.second)
            agenda_.push(p.first);
        }
    }
};

/**
 * C - typ kategorii (odpowiada symbolowi terminalnemu/nieterminalnemu z tradycyjnej
 *     gramatyki bezkontekstowej
 * S - typ oceny (np. int albo double)
 * V - typ wariantu (z ka¿d± kategori± typu C w wyniku parsowania mo¿e byæ zwi±zanych
 *     kilka wariantów, ustalanie wariantów odbywa siê po parsowaniu, za pomoc±
 *     osobnego mechanizmu)
 * R - typ regu³y; regu³a powinna udostêpniaæ funkcjê category() zwracaj±c±
 *     kategoriê "po lewej stronie" regu³y
 * K - typ kombinatora
 * A - typ agendy
 * I - rodzaj indeksu na³o¿onego na krawêdzie, domy¶lnie indeksujemy zarówno
 *     wchodz±ce, jak i wychodz±ce
 */
template<typename C, typename S, typename V, typename R, typename K, typename A,
     template<class,class> class I=simple_marked_edges_index>
class agenda_parser
{
public:
    typedef chart<C,S,V,R,I> chart_type;

    agenda_parser(chart_type& chart, K& combinator, A& agenda);

    void run();

private:
    void fill_();
    void go_();

    void apply_unary_rules_(typename chart_type::edge_descriptor e);
    void apply_binary_rules_(typename chart_type::edge_descriptor e, S min_score);
    void apply_binary_rules_for_pair_(
    typename chart_type::edge_descriptor e,
    typename chart_type::edge_descriptor f);

    chart_type& chart_;
    K& combinator_;
    A& agenda_;
};

#endif

