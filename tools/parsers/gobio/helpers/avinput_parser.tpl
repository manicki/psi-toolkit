#ifndef AVINPUT_PARSER_TPL_HDR
#define AVINPUT_PARSER_TPL_HDR

#include "avinput_parser.hpp"
#include "avinput_grammar.hpp"

typedef char const*         iterator_t;
typedef tree_match<iterator_t> parse_tree_match_t;
typedef parse_tree_match_t::tree_iterator iter_t;


template<class C, class R, class T, class M, class H, class Q, class L>
bool avinput_parser<C,R,T,M,H,Q,L>::parse(const std::string& s)
{
    avinput_grammar avgrammar;
    tree_parse_info<> info = pt_parse(s.c_str(), avgrammar,  (ch_p(' ') | ch_p('\t')));

    walk_all_(info.trees.begin());

    return info.full;
}

template<class C, class R, class T, class M, class H, class Q, class L>
typename H::vertex_descriptor avinput_parser<C,R,T,M,H,Q,L>::get_vertex_(int vertex_ix)
{
    while(vertex_ix >= (signed int)vertices_.size())
    {
    typename H::vertex_descriptor vd = chart_.add_vertex();

    vertices_.push_back(vd);
    }

    return vertices_[vertex_ix];
}


template<class C, class R, class T, class M, class H, class Q, class L>
void avinput_parser<C,R,T,M,H,Q,L>::walk_all_(iter_t const& i)
{
    iter_t sub_i;
    for(sub_i=i->children.begin(); sub_i != i->children.end(); ++sub_i)
    walk_segment_(sub_i);
}

template<class C, class R, class T, class M, class H, class Q, class L>
void avinput_parser<C,R,T,M,H,Q,L>::walk_segment_(iter_t const& i)
{
    assert(i->value.id() == avinput_grammar::segmentID);

    assert(i->children.size() >= 2);

    iter_t start_i = i->children.begin();
    iter_t stop_i  = i->children.begin()+1;

    start_i = start_i->children.begin();
    stop_i  = stop_i->children.begin();

    assert(start_i->value.id() == avinput_grammar::integerID);
    assert(stop_i->value.id() == avinput_grammar::integerID);

    std::string start (start_i->value.begin(), start_i->value.end());
    std::string stop  (stop_i->value.begin(),  stop_i->value.end());

    int start_ix = strtol(start.c_str(), 0, 10);
    int stop_ix  = strtol(stop.c_str(), 0, 10);

    iter_t sub_i;

    int start_v = get_vertex_(start_ix);
    int stop_v  = get_vertex_(stop_ix);

    for(sub_i=i->children.begin()+2; sub_i != i->children.end(); ++sub_i)
    walk_node_(sub_i, start_v, stop_v);
}

template<class C, class R, class T, class M, class H, class Q, class L>
void avinput_parser<C,R,T,M,H,Q,L>::walk_node_(
    iter_t const& i,
    typename H::vertex_descriptor start_d,
    typename H::vertex_descriptor stop_d)
{
    assert(i->value.id() == avinput_grammar::nodeID);

    iter_t symbol_i = i->children.begin()->children.begin();

    std::string symbol(symbol_i->value.begin(), symbol_i->value.end());

    av_matrix<C, T> entry( converter_.symbol_reader(symbol) );

    int start_assignment_ix = 1;

    T category = master_.false_value();

    iter_t sub_i;

    double score = 0.0;

    if(i->children.size() > 0)
    {
    if((signed int)i->children.size() > start_assignment_ix
       &&
       (i->children.begin()+start_assignment_ix)->value.id()
       == avinput_grammar::categoryID)
    {
        // start_assignment_ix = ++start_assignment_ix; // nonsense! did you mind:
        ++start_assignment_ix;

        iter_t category_i = (i->children.begin()+1)->children.begin();

        std::string category_str(category_i->value.begin(), category_i->value.end());

        category = master_.from_string(category_str);
    }


    if((signed int)i->children.size() > start_assignment_ix
       && (i->children.begin()+start_assignment_ix)->value.id()
       == avinput_grammar::scoreID)
    {
        iter_t score_i = (i->children.begin()+start_assignment_ix)->children.begin();
        std::string score_str(score_i->value.begin(), score_i->value.end());
        score = atof(score_str.c_str());
        ++start_assignment_ix;
    }

    for(sub_i=i->children.begin()+start_assignment_ix; sub_i != i->children.end(); ++sub_i)
    {
        if(sub_i->value.id() == avinput_grammar::extraID)
        break;

        walk_assignment_(sub_i, entry);
    }
    }

    local_rules_.push_back(L(category));

    typename H::edge_descriptor new_edge = chart_.add_edge(
    start_d,
    stop_d,
    converter_.matrix_reader(entry),
    score,
    R(-local_rules_.size(), 0, score, converter_.matrix_reader(entry))).first;

    typename H::partition_iterator new_partition
    = chart_.edge_partitions(new_edge).second;
    --new_partition;

    // bool was_variant_added = false;

    if (i->children.size() > 0 && sub_i != i->children.end())
    {
        for ( ; sub_i != i->children.end(); ++sub_i)
        {
            typename bare_av_matrix<T>::type avm;
            /* double v_score = */ walk_extra_(sub_i, avm);
            // chart_.add_variant(new_edge, new_partition, avm, v_score + score);
            // was_variant_added = true;
        }
    }
/*
    if(!was_variant_added)
    {
        chart_.add_variant(new_edge, new_partition, typename bare_av_matrix<T>::type(), score);
    }
*/
}

template<class C, class R, class T, class M, class H, class Q, class L>
void avinput_parser<C,R,T,M,H,Q,L>::walk_assignment_(
    iter_t const& i,
    av_matrix<C,T>& avm)
{
    assert(i->value.id() == avinput_grammar::assignmentID);
    assert(i->children.size() == 2);

    iter_t attr_i = (i->children.begin())->children.begin();
    iter_t val_i  = (i->children.begin()+1)->children.begin();

    assert(attr_i->value.id() == avinput_grammar::attributeID);
    assert(val_i->value.id() == avinput_grammar::integerID
       || val_i->value.id() == avinput_grammar::symbolID);

    std::string attr(attr_i->value.begin(), attr_i->value.end());
    std::string val (val_i->value.begin(),  val_i->value.end());

    if(val_i->value.id() == avinput_grammar::integerID)
    {
    avm.set_attr(
        converter_.attribute_reader(attr),
        master_.from_int(strtol(val.c_str(), 0, 10)),
        master_.false_value());
    }
    else
    {
    avm.set_attr(
        converter_.attribute_reader(attr),
        master_.from_string(val),
        master_.false_value());
    }
}

template<class C, class R, class T, class M, class H, class Q, class L>
double avinput_parser<C,R,T,M,H,Q,L>::walk_extra_(
    iter_t const& i, typename bare_av_matrix<T>::type& avm)
{
    assert(i->value.id() == avinput_grammar::extraID);

    double score = 0.0;

    if(i->children.size() > 0)
    {
    int start_assignment_ix = 0;

    if((i->children.begin()+start_assignment_ix)->value.id()
       == avinput_grammar::scoreID)
    {
        iter_t score_i = (i->children.begin()+start_assignment_ix)->children.begin();
        std::string score_str(score_i->value.begin(), score_i->value.end());
        score = atof(score_str.c_str());
        ++start_assignment_ix;
    }

    iter_t sub_i;

    for(sub_i=i->children.begin()+start_assignment_ix; sub_i != i->children.end(); ++sub_i)
        walk_extra_assignment_(sub_i, avm);
    }
/*     else */
/*     { */
/*  if(i->value.id() == avinput_grammar::scoreID) */
/*  { */
/*      iter_t score_i = i; */
/*      std::string score_str(score_i->value.begin(), score_i->value.end());         */
/*      score = atof(score_str.c_str()); */
/*  } */
/*  else */
/*      walk_extra_assignment_(i, avm); */
/*     } */

    return score;
}

template<class C, class R, class T, class M, class H, class Q, class L>
void avinput_parser<C,R,T,M,H,Q,L>::walk_extra_assignment_(
    iter_t const& i, typename bare_av_matrix<T>::type& avm)
{
    assert(i->value.id() == avinput_grammar::assignmentID);
    assert(i->children.size() == 2);

    iter_t attr_i = (i->children.begin())->children.begin();
    iter_t val_i  = (i->children.begin()+1)->children.begin();

    assert(attr_i->value.id() == avinput_grammar::attributeID);
    assert(val_i->value.id() == avinput_grammar::integerID
       || val_i->value.id() == avinput_grammar::symbolID);

    std::string attr(attr_i->value.begin(), attr_i->value.end());
    std::string val (val_i->value.begin(),  val_i->value.end());

    if(val_i->value.id() == avinput_grammar::integerID)
    {
    avm.set_attr(
        converter_.extra_attribute_reader(attr),
        master_.from_int(strtol(val.c_str(), 0, 10)),
        master_.false_value());
    }
    else
    {
    avm.set_attr(
        converter_.extra_attribute_reader(attr),
        master_.from_string(val),
        master_.false_value());
    }
}

#endif
