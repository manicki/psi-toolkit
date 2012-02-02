#ifndef CHART_TPL_HDR
#define CHART_TPL_HDR

#include <vector>
#include <algorithm>
// warningi pod GCC 3.3.4
#include <boost/graph/topological_sort.hpp>
#include "chart.hpp"




template<class C, class S, class V, class R, template<class,class> class I>
typename chart<C,S,V,R,I>::vertex_descriptor chart<C,S,V,R,I>::add_vertex()
{
    vertex_descriptor vd = boost::add_vertex(graph_);

    return vd;
}

template<class C, class S, class V, class R, template<class,class> class I>
std::pair<typename chart<C,S,V,R,I>::edge_descriptor,bool> chart<C,S,V,R,I>::add_edge(
	vertex_descriptor u, 
	vertex_descriptor v,
	const C& category,
	S score,
	R rule)
{
    std::pair<edge_descriptor,bool> p = 
	add_edge_(u, v, category, score);

    add_partition(p.first, score, rule);

    return p;
}

template<class C, class S, class V, class R, template<class,class> class I>
std::pair<typename chart<C,S,V,R,I>::edge_descriptor,bool> chart<C,S,V,R,I>::add_edge(
	vertex_descriptor u, 
	vertex_descriptor v,
	const C& category,
	S score,
	R rule,
	edge_descriptor link)
{
    std::pair<edge_descriptor,bool> p = 
	add_edge_(u, v, category, score);

    add_partition(p.first, score, rule, link);

    return p;
}

template<class C, class S, class V, class R, template<class,class> class I>
std::pair<typename chart<C,S,V,R,I>::edge_descriptor,bool> chart<C,S,V,R,I>::add_edge(
	vertex_descriptor u, 
	vertex_descriptor v,
	const C& category,
	S score,
	R rule,
	edge_descriptor  left_link,
	edge_descriptor right_link)
{
    std::pair<edge_descriptor,bool> p = 
	add_edge_(u, v, category, score);

    add_partition(p.first, score, rule, left_link, right_link);

    return p;
}

template<class C, class S, class V, class R, template<class,class> class I>
std::pair<typename chart<C,S,V,R,I>::edge_descriptor,bool> chart<C,S,V,R,I>::add_edge_(
	vertex_descriptor u, 
	vertex_descriptor v,
	const C& category,
	S score)
{
    std::pair<vertex_descriptor,vertex_descriptor> vpair(u,v);
    
    std::pair<std::pair<vertex_descriptor,vertex_descriptor>, C> hkey(vpair, category);

    // dodajemy pusty edge_descriptor(), właściwe dodanie
    // nastąpi później
    std::pair<vvc_hash_iterator_type_, bool> insert_result
	(vvc_hash_.insert(std::pair<
			  std::pair<std::pair<vertex_descriptor,vertex_descriptor>, C>,
			  edge_descriptor>(hkey, edge_descriptor())));

    if(insert_result.second) {
        if (!edge_counter_hash_[vpair]) {
            edge_counter_hash_[vpair] = 1;
        } else {
            ++edge_counter_hash_[vpair];
        }
        return std::pair<edge_descriptor,bool>(
            ((*insert_result.first).second
             = boost::add_edge(u, v, edge_entry(category, score), graph_).first),
            true);
    } else {
        return std::pair<edge_descriptor,bool>(
            (*insert_result.first).second,
            false);
    }
}

template<class C, class S, class V, class R, template<class,class> class I>
void chart<C,S,V,R,I>::remove_edge(
	edge_descriptor edge)
{
    boost::remove_edge(edge, graph_);
}

template<class C, class S, class V, class R, template<class,class> class I>
int chart<C,S,V,R,I>::nb_edges(
    vertex_descriptor u, 
    vertex_descriptor v)
{
    return edge_counter_hash_[std::pair<vertex_descriptor,vertex_descriptor>(u,v)];
}

template<class C, class S, class V, class R, template<class,class> class I>
typename chart<C,S,V,R,I>::edge_descriptor chart<C,S,V,R,I>::add_partition(
	edge_descriptor edge,
	S score,
	R rule)
{
    edge_entry& entry = graph_[edge];

    assert(&entry);

    if(score > entry.score)
	entry.score = score;

    entry.partitions.push_back(partition(score, rule));

    return edge;
}


template<class C, class S, class V, class R, template<class,class> class I>
typename chart<C,S,V,R,I>::edge_descriptor chart<C,S,V,R,I>::add_partition(
	edge_descriptor edge,
	S score,
	R rule,
	edge_descriptor link)
{
    edge_entry& entry = graph_[edge];

    assert(&entry);

    if(score > entry.score)
	entry.score = score;

    entry.partitions.push_back(partition(score, rule, link));

    return edge;
}

template<class C, class S, class V, class R, template<class,class> class I>
typename chart<C,S,V,R,I>::edge_descriptor chart<C,S,V,R,I>::add_partition(
	edge_descriptor edge,
	S score,
	R rule,
	edge_descriptor left_link,
	edge_descriptor right_link)
{
    edge_entry& entry = graph_[edge];

    assert(&entry);

    if(score > entry.score)
	entry.score = score;

    entry.partitions.push_back(partition(score, rule, left_link, right_link));

    return edge;
}

template<class C, class S, class V, class R, template<class,class> class I>
typename chart<C,S,V,R,I>::edge_descriptor chart<C,S,V,R,I>::mark_edge(
    edge_descriptor edge)
{
    graph_[source(edge, graph_)].marked_edges_index.mark_out_edge(edge, edge_category(edge));
    graph_[target(edge, graph_)].marked_edges_index.mark_in_edge(edge, edge_category(edge));

    return edge;
}

template<class C, class S, class V, class R, template<class,class> class I>
std::pair<typename chart<C,S,V,R,I>::vertex_iterator,
	  typename chart<C,S,V,R,I>::vertex_iterator>
chart<C,S,V,R,I>::vertices() const
{
    return boost::vertices(graph_);
}

template<class C, class S, class V, class R, template<class,class> class I>
size_t chart<C,S,V,R,I>::nb_vertices() const
{
    std::pair<
	vertex_iterator,
	vertex_iterator> vxits 
	= vertices();

    size_t r = 0;
    vertex_iterator vxit = vxits.first;
    while(vxit != vxits.second)
    {
	++r;
	++vxit;
    }
    
    return r;
}


template<class C, class S, class V, class R, template<class,class> class I>
std::pair<typename chart<C,S,V,R,I>::out_edge_iterator,
	  typename chart<C,S,V,R,I>::out_edge_iterator>
chart<C,S,V,R,I>::out_edges(vertex_descriptor vertex) const
{
    return boost::out_edges(vertex, graph_);
}

template<class C, class S, class V, class R, template<class,class> class I>
std::pair<typename chart<C,S,V,R,I>::in_edge_iterator,
	  typename chart<C,S,V,R,I>::in_edge_iterator>
chart<C,S,V,R,I>::in_edges(vertex_descriptor vertex) const
{
    return boost::in_edges(vertex, graph_);
}

template<class C, class S, class V, class R, template<class,class> class I>
typename chart<C,S,V,R,I>::marked_edges_index_type&
chart<C,S,V,R,I>::marked_edges_index(vertex_descriptor vertex)
{
    return graph_[vertex].marked_edges_index;
}

template<class C, class S, class V, class R, template<class,class> class I>
typename chart<C,S,V,R,I>::vertex_descriptor chart<C,S,V,R,I>::edge_source(
    edge_descriptor edge)
{
    return boost::source(edge, graph_);
}

template<class C, class S, class V, class R, template<class,class> class I>
typename chart<C,S,V,R,I>::vertex_descriptor chart<C,S,V,R,I>::edge_target(
    edge_descriptor edge)
{
    return boost::target(edge, graph_);
}

template<class C, class S, class V, class R, template<class,class> class I>
typename param_type<C>::type chart<C,S,V,R,I>::edge_category(
    edge_descriptor edge)
{
    return graph_[edge].category;
}

template<class C, class S, class V, class R, template<class,class> class I>
S chart<C,S,V,R,I>::edge_score(edge_descriptor edge) const
{
    return graph_[edge].score;
}

template<class C, class S, class V, class R, template<class,class> class I>
bool chart<C,S,V,R,I>::edge_accommodated(edge_descriptor edge) const
{
    return graph_[edge].is_accommodated;
}

template<class C, class S, class V, class R, template<class,class> class I>
void chart<C,S,V,R,I>::mark_edge_as_accommodated(edge_descriptor edge)
{
    graph_[edge].is_accommodated = true;
}


template<class C, class S, class V, class R, template<class,class> class I>
std::pair<typename chart<C,S,V,R,I>::partition_iterator, 
	  typename chart<C,S,V,R,I>::partition_iterator> 
chart<C,S,V,R,I>::edge_partitions(
    edge_descriptor edge)
{
    std::list<partition>& partition_vector = graph_[edge].partitions;

    return std::pair<
	partition_iterator, 
	partition_iterator>(
	    partition_vector.begin(),
	    partition_vector.end());
}

template<class C, class S, class V, class R, template<class,class> class I>
std::pair<typename chart<C,S,V,R,I>::variant_iterator, 
	  typename chart<C,S,V,R,I>::variant_iterator> 
chart<C,S,V,R,I>::edge_variants(
    edge_descriptor edge)
{
    std::vector<variant>& variant_vector = graph_[edge].variants;

    return std::pair<
	variant_iterator, 
	variant_iterator>(
	    variant_vector.begin(),
	    variant_vector.end());
}

template<class V>
class the_same_variant_category
{
private:
    const typename V::variant_category_type& cat_;

public:
    the_same_variant_category(const V& model):cat_(model.variant_category) {}
    bool operator() (const V& v)
	{ return v.variant_category == cat_; }
};

template<class C, class S, class V, class R, template<class,class> class I>
void chart<C,S,V,R,I>::remove_duplicate_variants(edge_descriptor edge)
{
    if(graph_[edge].variants.size() <= 1)
	return;
    
    std::vector<variant> new_variants;
    variant_iterator i;
    i = graph_[edge].variants.begin();

    for(;i != graph_[edge].variants.end();++i)
    {
	the_same_variant_category<variant> epred(*i);

	variant_iterator prev = 
	    find_if(
		new_variants.begin(),
		new_variants.end(),
		epred);

	if(prev != new_variants.end())
	{
	    if((*i).score > (*prev).score)
		*prev = *i;
	}
	else
	    new_variants.push_back(*i);
    }
    
    graph_[edge].variants = new_variants;
}

template<class C, class S, class V, class R, template<class,class> class I>
void chart<C,S,V,R,I>::sort_variants(edge_descriptor edge, size_t limit)
{
    remove_duplicate_variants(edge);

    std::stable_sort(
	    graph_[edge].variants.begin(),
	    graph_[edge].variants.end());

    if(graph_[edge].variants.size() > limit && limit != 0)
	graph_[edge].variants.erase(
	    graph_[edge].variants.begin() + limit,
	    graph_[edge].variants.end());
}

template<class C, class S, class V, class R, template<class,class> class I>
void chart<C,S,V,R,I>::add_variant(
    edge_descriptor edge,
    partition_iterator piter, 
    const V& new_variant,
    S score)
{
    std::vector<variant>& variants = graph_[edge].variants;
    variants.push_back(variant_type());
    variants.back().variant_category = new_variant;
    variants.back().score = score;
    variants.back().pit = piter;
}

template<class C, class S, class V, class R, template<class,class> class I>
void chart<C,S,V,R,I>::add_variant(
    edge_descriptor edge,
    partition_iterator piter,
    const V& new_variant,
    S score,
    variant_iterator    link_vit)
{
    std::vector<variant>& variants = graph_[edge].variants;
    variants.push_back(variant_type());
    variants.back().variant_category = new_variant;
    variants.back().score = score;
    variants.back().pit = piter;

    variants.back().links.push_back(link_vit);
}


template<class C, class S, class V, class R, template<class,class> class I>
void chart<C,S,V,R,I>::add_variant(
    edge_descriptor edge,
    partition_iterator piter,
    const V& new_variant,
    S score,
    variant_iterator    left_link_vit,
    variant_iterator    right_link_vit)
{
    std::vector<variant>& variants = graph_[edge].variants;
    variants.push_back(variant_type());
    variants.back().variant_category = new_variant;
    variants.back().score = score;
    variants.back().pit = piter;

    variants.back().links.reserve(2);
    variants.back().links.push_back(left_link_vit);
    variants.back().links.push_back(right_link_vit);
}


template<class C, class S, class V, class R, template<class,class> class I>
R chart<C,S,V,R,I>::partition_rule(
    partition_iterator piter)
{
    return (*piter).rule;
}

template<class C, class S, class V, class R, template<class,class> class I>
std::vector<typename chart<C,S,V,R,I>::edge_descriptor>& chart<C,S,V,R,I>::partition_links(
    partition_iterator piter)
{
    return (*piter).links;
}

template<class C, class S, class V, class R, template<class,class> class I>
typename chart<C,S,V,R,I>::partition_iterator chart<C,S,V,R,I>::variant_partition(variant_iterator vit)
{
    return (*vit).pit;
}

template<class C, class S, class V, class R, template<class,class> class I>
typename param_type<V>::type chart<C,S,V,R,I>::edge_variant_category(variant_iterator vit)
{
    return (*vit).variant_category;
}

template<class C, class S, class V, class R, template<class,class> class I>
S chart<C,S,V,R,I>::variant_score(variant_iterator vit)
{
    return (*vit).score;
}

template<class C, class S, class V, class R, template<class,class> class I>
std::vector<typename chart<C,S,V,R,I>::variant_iterator>&
chart<C,S,V,R,I>::variant_links(variant_iterator vit)
{
    return (*vit).links;
}

template<class C, class S, class V, class R, template<class,class> class I>
void chart<C,S,V,R,I>::sort_topologically()
{
    std::vector<vertex_descriptor> vc;
    boost::topological_sort(graph_, std::back_inserter(vc));
    
    size_t ix = 0;

    for(typename std::vector<vertex_descriptor>::reverse_iterator
	    ii = vc.rbegin();
	ii != vc.rend();
	++ii)
    {
	sorted_vertices_.push_back(*ii);
	topological_order_[(*ii)] = ix++;
    }
}

template<class C, class S, class V, class R, template<class,class> class I>
size_t chart<C,S,V,R,I>::topological_index(vertex_descriptor vertex) const
{
    assert(topological_order_.count(vertex));

    return (*(topological_order_.find(vertex))).second;
}

template<class C, class S, class V, class R, template<class,class> class I>
typename chart<C,S,V,R,I>::vertex_descriptor chart<C,S,V,R,I>::vertex_by_index(size_t i) const
{
    assert(i < sorted_vertices_.size());

    return sorted_vertices_[i];
}

template<class C, class S, class V, class R, template<class,class> class I>
size_t chart<C,S,V,R,I>::topological_count() const
{
    return sorted_vertices_.size();
}

#endif
