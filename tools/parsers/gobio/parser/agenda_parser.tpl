#ifndef AGENDA_PARSER_TPL_HDR
#define AGENDA_PARSER_TPL_HDR

#include "agenda_parser.hpp"


template<typename C, typename S, typename V, typename R, typename K, typename A,
     template<class,class> class I>
agenda_parser<C,S,V,R,K,A,I>::agenda_parser(
    chart_type& chart,
    K& combinator,
    A& agenda)
    :chart_(chart),
    combinator_(combinator),
    agenda_(agenda)
{
}


template<typename C, typename S, typename V, typename R, typename K, typename A,
     template<class,class> class I>
void agenda_parser<C,S,V,R,K,A,I>::run()
{
    fill_();
    go_();
}

template<typename C, typename S, typename V, typename R, typename K, typename A,
     template<class,class> class I>
void agenda_parser<C,S,V,R,K,A,I>::fill_()
{
    typename chart_type::vertex_iterator vertex_it = chart_.vertices();

    S initial_min_score = combinator_.get_limit_min_score(0);

    while (vertex_it.hasNext()) {
        typename chart_type::out_edge_iterator edge_it = chart_.out_edges(vertex_it.next());

        while (edge_it.hasNext()) {
            Lattice::EdgeDescriptor edge = edge_it.next();
            if (chart_.edge_score(edge) >= initial_min_score
            && combinator_.is_useful(chart_.edge_category(edge))) {
            agenda_.push(edge);
            }
        }
    }
}

template<typename C, typename S, typename V, typename R, typename K, typename A,
     template<class,class> class I>
void agenda_parser<C,S,V,R,K,A,I>::go_()
{
    assert(combinator_.get_nb_limits() > 0);

//     unsigned int nb_vertices = chart_.nb_vertices();

//     unsigned int log_2_nb_vertices = 1;
//     unsigned int b = 1;
//     while(b < nb_vertices)
//     {
//  b <<= 1;
//  ++log_2_nb_vertices;
//     }

    unsigned int limit_level = 0;
    int current_threshold = combinator_.get_limit_threshold(0);
    S current_min_score = combinator_.get_limit_min_score(0);
    int nb_turns = 0;

    while(!agenda_.empty())
    {
        // wybranie bieżącej krawędzi
        typename chart_type::edge_descriptor e = agenda_.top();
        agenda_.pop();

        // liczba obrotów może przyrastać liniowo
        // poprzedni warunek: ((++nb_turns) >> log_2_nb_vertices)
        if((++nb_turns) + (signed int)agenda_.size() > current_threshold)
        {
            if(++limit_level >= combinator_.get_nb_limits())
            break;

            current_threshold = combinator_.get_limit_threshold(limit_level);
            current_min_score = combinator_.get_limit_min_score(limit_level);
        }

        if (chart_.edge_score(e) < current_min_score) {
            continue;
        }

        if (agenda_.is_astar()) {
            if (
                chart_.topological_index(chart_.edge_source(e))==0 &&
                chart_.topological_index(chart_.edge_target(e))==chart_.topological_count()-1
            ) {
                if (combinator_.is_root(chart_.edge_category(e))) {
                    break;
                }
            }
        }


        // zastosowanie reguł unarnych
        apply_unary_rules_(e);

        // zastosowanie reguł binarnych
        apply_binary_rules_(e, current_min_score);

        // zaznaczenie bieżącej krawędzi
        chart_.mark_edge(e);
    }

    combinator_.clear_parser_caches();
}

template<typename C, typename S, typename V, typename R, typename K, typename A,
     template<class,class> class I>
void agenda_parser<C,S,V,R,K,A,I>::apply_unary_rules_(
    typename chart_type::edge_descriptor e)
{
    typename chart_type::vertex_descriptor source = chart_.edge_source(e);
    typename chart_type::vertex_descriptor target = chart_.edge_target(e);

    // if (chart_.nb_edges(source,target) > combinator_.get_limit_edges(chart_.topological_index(target)-chart_.topological_index(source))) return;

    std::vector<R> rules(combinator_.combine_unary(chart_.edge_category(e)));

    S escore = chart_.edge_score(e);

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
        e);

    if(p.second)
        agenda_.push(p.first);
    }
}

template<typename C, typename S, typename V, typename R, typename K, typename A,
     template<class,class> class I>
void agenda_parser<C,S,V,R,K,A,I>::apply_binary_rules_(
    typename chart_type::edge_descriptor e, S min_score)
{
    apply_binary_rules_algorithm<C,S,V,R,K,A,I>::apply(
    e, min_score, chart_, combinator_, agenda_);
}


/* template<typename C, typename S, typename V, typename R, typename K, typename A, */
/*   template<class,class> class I> */
/* void agenda_parser<C,S,V,R,K,A,I>::apply_binary_rules_( */
/*     typename chart_type::edge_descriptor e) */
/* { */
/*     // zastosowanie reguł binarnych z bieżącą krawędzią po prawej stronie */
/*     typename chart_type::vertex_descriptor source = chart_.edge_source(e); */

/*     std::pair< */
/*  typename chart_type::marked_in_edge_iterator, */
/*  typename chart_type::marked_in_edge_iterator> ins =  */
/*  chart_.marked_edges_index(source).marked_in_edges(); */

/*     for(typename chart_type::marked_in_edge_iterator it = ins.first; */
/*  it != ins.second; */
/*  ++it) */
/*  apply_binary_rules_for_pair_((*it), e); */

/*     // zastosowanie reguł binarnych z bieżącą krawędzią po lewej stronie */
/*     typename chart_type::vertex_descriptor target = chart_.edge_target(e); */

/*     std::pair< */
/*  typename chart_type::marked_out_edge_iterator, */
/*  typename chart_type::marked_out_edge_iterator> outs = */
/*  chart_.marked_edges_index(target).marked_out_edges(); */

/*     for(typename chart_type::marked_out_edge_iterator it = outs.first; */
/*  it != outs.second; */
/*  ++it) */
/*  apply_binary_rules_for_pair_(e,(*it)); */
/* } */

/* template<typename C, typename S, typename V, typename R, typename K, typename A, */
/*   template<class,class> class I> */
/* void agenda_parser<C,S,V,R,K,A,I>::apply_binary_rules_for_pair_( */
/*     typename chart_type::edge_descriptor e, */
/*     typename chart_type::edge_descriptor f) */
/* { */
/*     std::vector<R> rules(combinator_.combine_binary( */
/*  chart_.edge_category(e), */
/*  chart_.edge_category(f))); */

/*     if(rules.empty()) */
/*  return; */

/*     typename chart_type::vertex_descriptor source = chart_.edge_source(e); */
/*     typename chart_type::vertex_descriptor target = chart_.edge_target(f); */
/*     S escore = chart_.edge_score(e) + chart_.edge_score(f); */

/*     for(typename std::vector<R>::iterator it = rules.begin(); */
/*  it != rules.end(); */
/*  ++it) */
/*     { */
/*  std::pair<typename chart_type::edge_descriptor, bool> p */
/*      = chart_.add_edge( */
/*      source, */
/*      target, */
/*      (*it).category(), */
/*      escore + (*it).score(), */
/*      (*it), */
/*      e, */
/*      f); */

/*  if(p.second) */
/*      agenda_.push(p.first); */
/*     } */
/* } */

#endif
