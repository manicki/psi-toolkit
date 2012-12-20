#ifndef ACCOMMODATER_TPL_HPP
#define ACCOMMODATER_TPL_HPP

#include "accommodater.hpp"

template<class H, class K>
binary_accommodater<H,K>::binary_accommodater(H& chart, K& combinator, size_t limit):
    chart_(chart), combinator_(combinator), limit_(limit)
{
}

template<class H, class K>
void binary_accommodater<H,K>::accommodate(typename H::edge_descriptor edge)
{
    if (accommodated_[edge])
    return;

    std::pair<
    typename H::partition_iterator,
    typename H::partition_iterator> pits
    = chart_.edge_partitions(edge);

    // bool lexical_entry_found = false;

    for(typename H::partition_iterator pit = pits.first;
    pit != pits.second;
    ++pit)
    {
    // if(combinator_.is_lexical(chart_.partition_rule_id(pit)))
        // lexical_entry_found = true;

    accommodate_partition_(edge, pit);
    }

    // jeśli dane pochodzą ze słownika nie wprowadzamy żadnych ograniczeń
    // chart_.sort_variants(edge, (lexical_entry_found ? 0 : limit_));

    accommodated_[edge] = true;
}

template<class H, class K>
void binary_accommodater<H,K>::accommodate_partition_(
    typename H::edge_descriptor edge, typename H::partition_iterator pit)
{
    if(combinator_.is_lexical(chart_.partition_rule_id(pit)))
    return;

    switch(chart_.partition_links_size(pit))
    {
    case 0:
    break;
    case 1:
    accommodate_unary_(edge, pit);
    break;
    case 2:
    accommodate_binary_(edge, pit);
    break;
    default:
    assert(0);
    }
}


template<class H, class K>
void binary_accommodater<H,K>::accommodate_unary_(
    typename H::edge_descriptor /* edge */, typename H::partition_iterator pit)
{
    Lattice::Partition::Iterator pli = chart_.partition_links_iterator(pit);
    if (pli.hasNext()) {
        accommodate(pli.next());
    }
/*
    std::pair<
    typename H::variant_iterator,
    typename H::variant_iterator> vits
    = chart_.edge_variants(plinks[0]);

    if(vits.first == vits.second)
    return;

    typename H::variant_category_type v_cat;
    typename H::score_type v_score;

    // jeśli akomodacja nie zależy od składnika wystarczy wziąć najlepszy wariant
    typename H::variant_iterator last_vit
    = (combinator_.is_variant_independent(chart_.partition_rule_id(pit), 0)
       ? vits.first+1
       : vits.second);

    for(typename H::variant_iterator vit = vits.first;
    vit != last_vit;
    ++vit)
    {
    typename H::category_type super_cat = chart_.edge_category(edge);

    if(combinator_.variant_combine_unary(
           super_cat,
           chart_.edge_category(plinks[0]),
           chart_.edge_variant_category(vit),
           chart_.partition_rule_id(pit),
           v_cat,
           v_score))
        chart_.add_variant(
        edge,
        pit,
        v_cat,
        v_score + chart_.variant_score(vit) + chart_.partition_rule_id(pit).score(),
        vit);
    }
// */
}

template<class H, class K>
void binary_accommodater<H,K>::accommodate_binary_(
    typename H::edge_descriptor /* edge */, typename H::partition_iterator pit)
{
    Lattice::Partition::Iterator pli = chart_.partition_links_iterator(pit);
    if (pli.hasNext()) {
        accommodate(pli.next());
    }
    if (pli.hasNext()) {
        accommodate(pli.next());
    }
/*
    std::pair<
    typename H::variant_iterator,
    typename H::variant_iterator> left_vits
    = chart_.edge_variants(plinks[0]);

    if(left_vits.first == left_vits.second)
    return;

    std::pair<
    typename H::variant_iterator,
    typename H::variant_iterator> right_vits
    = chart_.edge_variants(plinks[1]);

    if(right_vits.first == right_vits.second)
    return;

    typename H::variant_category_type v_cat;
    typename H::score_type v_score;

    // jeśli akomodacja nie zależy od składnika wystarczy wziąć najlepszy wariant
    typename H::variant_iterator left_last_vit
    = (combinator_.is_variant_independent(chart_.partition_rule_id(pit), 0)
       ? left_vits.first+1
       : left_vits.second);
    typename H::variant_iterator right_last_vit
    = (combinator_.is_variant_independent(chart_.partition_rule_id(pit), 1)
       ? right_vits.first+1
       : right_vits.second);

    for(typename H::variant_iterator left_vit = left_vits.first;
    left_vit != left_last_vit;
    ++left_vit)
    for(typename H::variant_iterator right_vit = right_vits.first;
        right_vit != right_last_vit;
        ++right_vit)
    {
        typename H::category_type super_cat = chart_.edge_category(edge);
        if(combinator_.variant_combine_binary(
           super_cat,
           chart_.edge_category(plinks[0]),
           chart_.edge_category(plinks[1]),
           chart_.edge_variant_category(left_vit),
           chart_.edge_variant_category(right_vit),
           chart_.partition_rule_id(pit),
           v_cat,
           v_score))
        chart_.add_variant(
            edge,
            pit,
            v_cat,
            v_score
            + chart_.variant_score(left_vit)
            + chart_.variant_score(right_vit)
            + chart_.partition_rule_id(pit).score(),
            left_vit,
            right_vit);
    }
// */
}


#endif
