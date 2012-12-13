#ifndef CHOOSER_TPL_HDR
#define CHOOSER_TPL_HDR

#include "chooser.hpp"
#include <algorithm>

#include "tree_branch.tpl"
#include "extract_tree.tpl"

#include <accommodater.tpl>

template<class Ch>
class sort_edges_predicate
{
private:
    Ch& chart_;

    int topo_length(const typename Ch::edge_descriptor& a)
    {
        return
        chart_.topological_index(chart_.edge_target(a))
        - chart_.topological_index(chart_.edge_source(a));
    }

public:
    sort_edges_predicate(Ch& chart):chart_(chart) {}

    bool operator()(const typename Ch::edge_descriptor& a,
            const typename Ch::edge_descriptor& b)
    {
        int tla = topo_length(a);
        int tlb = topo_length(b);

        return
        tlb < tla ||
        tlb == tla && chart_.edge_score(b) < chart_.edge_score(a);
    }
};

template<class Ch>
void sort_edges(Ch& chart, std::vector<typename Ch::edge_descriptor>& edges)
{
    std::stable_sort(edges.begin(), edges.end(), sort_edges_predicate<Ch>(chart));
};


template<class Ch, class K>
std::vector<typename Ch::edge_descriptor> longest_left_to_right_chooser<Ch,K>::go(Ch& chart,K& combinator,const std::vector<typename longest_left_to_right_chooser<Ch,K>::rule_holder>& local_rules)
{
    std::vector<typename Ch::edge_descriptor> r;

    typename Ch::vertex_descriptor v = *(chart.vertices().first);

    while(1)
    {
    std::vector<typename Ch::edge_descriptor> v_edges;

    std::pair<
        typename Ch::out_edge_iterator,
        typename Ch::out_edge_iterator> v_edges_its =
        chart.out_edges(v);

    for(typename Ch::out_edge_iterator it = v_edges_its.first;
        it != v_edges_its.second;
        ++it)
        v_edges.push_back(*it);

    if(v_edges.empty())
        break;

    sort_edges<Ch>(chart, v_edges);

    typename std::vector<typename Ch::edge_descriptor>::iterator ei = v_edges.begin();
    typename std::vector<typename Ch::edge_descriptor>::iterator last_chance_edge = v_edges.end();

    binary_accommodater<Ch, K> accommodater(chart, combinator, 32);

    while(ei != v_edges.end())
    {
        int topo_i = chart.topological_index(chart.edge_target(*ei));

        typename Ch::score_type max_preference;
        typename Ch::score_type max_score;
        typename std::vector<typename Ch::edge_descriptor>::iterator max_e;
        bool candidate_found = false;

        // w tej pętli przeglądamy wszystkie krawędzie o danej długości
        typename std::vector<typename Ch::edge_descriptor>::iterator ej
        = ei;
        for( ; ej != v_edges.end() ; ++ej)
        {
        int topo_j = chart.topological_index(chart.edge_target(*ej));

        assert(topo_j <= topo_i);

        if(topo_j != topo_i)
            break;

        last_chance_edge = ej;

        if(combinator.could_be_final(chart.edge_category(*ej)))
        {
            accommodater.accommodate(*ej);

            std::pair<
            typename Ch::variant_iterator,
            typename Ch::variant_iterator> vits
            = chart.edge_variants(*ej);

            typename Ch::variant_iterator final_vit = vits.first;

            while(final_vit != vits.second
              && !combinator.could_be_final_variant(
                  chart.edge_category(*ej),
                  chart.edge_variant_category(final_vit)))
            ++final_vit;

            if(final_vit != vits.second)
            {
            boost::shared_ptr<
                tree_branch<typename K::atom_type, Ch,typename K::equivalent_type> > tb
                = extract_tree_branch<typename K::atom_type, Ch, K>(
                chart,
                *ej,
                combinator,
                local_rules);

            if(tb)
            {
                typename Ch::score_type j_preference
                = combinator.get_preference(tb->root());
                typename Ch::score_type j_score
                = chart.variant_score(final_vit);

                if(!candidate_found ||
                   j_preference > max_preference ||
                   (j_preference == max_preference && j_score > max_score))
                {
                candidate_found = true;
                max_preference = j_preference;
                max_score = j_score;
                max_e = ej;
                }
            }
            }
        }
        }

        if(candidate_found)
        {
        ei = max_e;
        break;
        }

        ei = ej;
    }

    if (ei == v_edges.end())
    {
        // ' - ' p=>e ' - '
        // ' - ' e=>p ' - '
        // ' - ' d=>p ' - '
        assert(last_chance_edge != v_edges.end());
        ei = last_chance_edge;
    }
    else
    {
        assert(chart.edge_partitions(*ei).second !=
           chart.edge_partitions(*ei).first);

    }

    r.push_back(*ei);

    assert(v != chart.edge_target(*ei));

    v = chart.edge_target(*ei);
    }

    return r;
}

#endif
