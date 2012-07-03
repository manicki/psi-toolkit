#ifndef CHART_TPL_HDR
#define CHART_TPL_HDR

// warningi pod GCC 3.3.4
// #include <boost/graph/topological_sort.hpp>

#include "chart.hpp"


template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
chart<C,S,V,R,I>::chart(
    Lattice & lattice,
    AV_AI_Converter & av_ai_converter
) :
    lattice_(lattice),
    av_ai_converter_(av_ai_converter),
    gobioTag_(lattice.getLayerTagManager().createSingletonTagCollection("parse")),
    tagMask_(lattice.getLayerTagManager().anyTag())
{
    std::vector<LayerTagCollection> altTags;
    altTags.push_back(lattice.getLayerTagManager().createSingletonTagCollection("form"));
    altTags.push_back(lattice.getLayerTagManager().createSingletonTagCollection("lemma"));
    altTags.push_back(lattice.getLayerTagManager().createSingletonTagCollection("lexeme"));
    altTags.push_back(lattice.getLayerTagManager().createSingletonTagCollection("parse"));
    altTags.push_back(lattice.getLayerTagManager().createSingletonTagCollection("token"));
    setTagMask(lattice.getLayerTagManager().getAlternativeMask(altTags));
}


template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
typename chart<C,S,V,R,I>::vertex_descriptor chart<C,S,V,R,I>::add_vertex()
{
    vertex_descriptor vd = lattice_.addLooseVertex();
    return vd;
}


template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
std::pair<typename chart<C,S,V,R,I>::edge_descriptor,bool> chart<C,S,V,R,I>::add_edge(
    vertex_descriptor u,
    vertex_descriptor v,
    const category_type& category,
    score_type score,
    rule_type rule)
{
    try {
        AnnotationItem ai = av_ai_converter_.toAnnotationItem(category);
        int num1 = lattice_.countEdges(u, v);
        Lattice::EdgeDescriptor result = lattice_.addEdge(
            u,
            v,
            ai,
            getGobioTag_(),
            Lattice::EdgeSequence(),
            score,
            rule.rule_no()
        );
        int num2 = lattice_.countEdges(u, v);
        return std::pair<edge_descriptor,bool>(result, num2 - num1 > 0);
    } catch (EdgeSelfReferenceException) {
        return std::pair<edge_descriptor,bool>(Lattice::EdgeDescriptor(), false);
    }
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
std::pair<typename chart<C,S,V,R,I>::edge_descriptor,bool> chart<C,S,V,R,I>::add_edge(
    vertex_descriptor u,
    vertex_descriptor v,
    const category_type& category,
    score_type score,
    rule_type rule,
    edge_descriptor link)
{
    try {
        AnnotationItem ai = av_ai_converter_.toAnnotationItem(category);
        Lattice::EdgeSequence::Builder builder(lattice_);
        builder.addEdge(link);
        int num1 = lattice_.countEdges(u, v);
        Lattice::EdgeDescriptor result = lattice_.addEdge(
            u,
            v,
            ai,
            getGobioTag_(),
            builder.build(),
            score,
            rule.rule_no()
        );
        int num2 = lattice_.countEdges(u, v);
        return std::pair<edge_descriptor,bool>(result, num2 - num1 > 0);
    } catch (EdgeSelfReferenceException) {
        return std::pair<edge_descriptor,bool>(Lattice::EdgeDescriptor(), false);
    }
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
std::pair<typename chart<C,S,V,R,I>::edge_descriptor,bool> chart<C,S,V,R,I>::add_edge(
    vertex_descriptor u,
    vertex_descriptor v,
    const category_type& category,
    score_type score,
    rule_type rule,
    edge_descriptor left_link,
    edge_descriptor right_link)
{
    try {
        AnnotationItem ai = av_ai_converter_.toAnnotationItem(category);
        Lattice::EdgeSequence::Builder builder(lattice_);
        builder.addEdge(left_link);
        builder.addEdge(right_link);
        int num1 = lattice_.countEdges(u, v);
        Lattice::EdgeDescriptor result = lattice_.addEdge(
            u,
            v,
            ai,
            getGobioTag_(),
            builder.build(),
            score,
            rule.rule_no()
        );
        int num2 = lattice_.countEdges(u, v);
        return std::pair<edge_descriptor,bool>(result, num2 - num1 > 0);
    } catch (EdgeSelfReferenceException) {
        return std::pair<edge_descriptor,bool>(Lattice::EdgeDescriptor(), false);
    }
}


template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
void chart<C,S,V,R,I>::remove_edge(
    edge_descriptor edge)
{
    lattice_.discard(edge);
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
int chart<C,S,V,R,I>::nb_edges(
    vertex_descriptor u,
    vertex_descriptor v)
{
    return lattice_.countEdges(u, v);
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
typename chart<C,S,V,R,I>::edge_descriptor chart<C,S,V,R,I>::add_partition(
    edge_descriptor edge,
    score_type score,
    rule_type rule)
{
    return lattice_.addPartitionToEdge(
        edge,
        getGobioTag_(),
        Lattice::EdgeSequence(),
        score,
        rule.rule_no()
    );
}


template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
typename chart<C,S,V,R,I>::edge_descriptor chart<C,S,V,R,I>::add_partition(
    edge_descriptor edge,
    score_type score,
    rule_type rule,
    edge_descriptor link)
{
    Lattice::EdgeSequence::Builder builder(lattice_);
    builder.addEdge(link);
    return lattice_.addPartitionToEdge(
        edge,
        getGobioTag_(),
        builder.build(),
        score,
        rule.rule_no()
    );
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
typename chart<C,S,V,R,I>::edge_descriptor chart<C,S,V,R,I>::add_partition(
    edge_descriptor edge,
    score_type score,
    rule_type rule,
    edge_descriptor left_link,
    edge_descriptor right_link)
{
    Lattice::EdgeSequence::Builder builder(lattice_);
    builder.addEdge(left_link);
    builder.addEdge(right_link);
    return lattice_.addPartitionToEdge(
        edge,
        getGobioTag_(),
        builder.build(),
        score,
        rule.rule_no()
    );
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
typename chart<C,S,V,R,I>::edge_descriptor chart<C,S,V,R,I>::mark_edge(
    edge_descriptor edge)
{
    markedEdgesIndexes_[lattice_.getEdgeSource(edge)].mark_out_edge(edge, edge_category(edge));
    markedEdgesIndexes_[lattice_.getEdgeTarget(edge)].mark_in_edge(edge, edge_category(edge));

    return edge;
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
typename chart<C,S,V,R,I>::vertex_iterator chart<C,S,V,R,I>::vertices()
{
    return Lattice::VertexIterator(lattice_, false);
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
size_t chart<C,S,V,R,I>::nb_vertices() const
{
    return lattice_.countAllVertices();
}


template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
typename chart<C,S,V,R,I>::out_edge_iterator
chart<C,S,V,R,I>::out_edges(vertex_descriptor vertex)
{
    return lattice_.outEdges(vertex, getTagMask_());
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
typename chart<C,S,V,R,I>::in_edge_iterator
chart<C,S,V,R,I>::in_edges(vertex_descriptor vertex)
{
    return lattice_.inEdges(vertex, getTagMask_());
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
typename chart<C,S,V,R,I>::marked_edges_index_type&
chart<C,S,V,R,I>::marked_edges_index(vertex_descriptor vertex)
{
    return markedEdgesIndexes_[vertex];
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
typename chart<C,S,V,R,I>::vertex_descriptor chart<C,S,V,R,I>::edge_source(
    edge_descriptor edge)
{
    return lattice_.getEdgeSource(edge);
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
typename chart<C,S,V,R,I>::vertex_descriptor chart<C,S,V,R,I>::edge_target(
    edge_descriptor edge)
{
    return lattice_.getEdgeTarget(edge);
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
typename chart<C,S,V,R,I>::category_type chart<C,S,V,R,I>::edge_category(
    edge_descriptor edge)
{
    return av_ai_converter_.toAVMatrix<category_type>(lattice_.getEdgeAnnotationItem(edge));
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
typename chart<C,S,V,R,I>::score_type chart<C,S,V,R,I>::edge_score(edge_descriptor edge) const
{
    return lattice_.getEdgeScore(edge);
}

/*
template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
bool chart<C,S,V,R,I>::edge_accommodated(edge_descriptor edge) const
{
    return graph_[edge].is_accommodated;
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
void chart<C,S,V,R,I>::mark_edge_as_accommodated(edge_descriptor edge)
{
    graph_[edge].is_accommodated = true;
}
*/

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
std::pair<typename chart<C,S,V,R,I>::partition_iterator,
      typename chart<C,S,V,R,I>::partition_iterator>
chart<C,S,V,R,I>::edge_partitions(
    edge_descriptor edge)
{
    std::list<Lattice::Partition> partition_vector = lattice_.getEdgePartitions(edge);

    return std::pair<
    partition_iterator,
    partition_iterator>(
        partition_vector.begin(),
        partition_vector.end());
}

/*
template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
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

template<typename V>
class the_same_variant_category
{
private:
    const typename V::variant_category_type& cat_;

public:
    the_same_variant_category(const V& model):cat_(model.variant_category) {}
    bool operator() (const V& v)
    { return v.variant_category == cat_; }
};

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
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

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
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

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
void chart<C,S,V,R,I>::add_variant(
    edge_descriptor edge,
    partition_iterator piter,
    const V& new_variant,
    score_type score)
{
    std::vector<variant>& variants = graph_[edge].variants;
    variants.push_back(variant_type());
    variants.back().variant_category = new_variant;
    variants.back().score = score;
    variants.back().pit = piter;
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
void chart<C,S,V,R,I>::add_variant(
    edge_descriptor edge,
    partition_iterator piter,
    const V& new_variant,
    score_type score,
    variant_iterator    link_vit)
{
    std::vector<variant>& variants = graph_[edge].variants;
    variants.push_back(variant_type());
    variants.back().variant_category = new_variant;
    variants.back().score = score;
    variants.back().pit = piter;

    variants.back().links.push_back(link_vit);
}


template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
void chart<C,S,V,R,I>::add_variant(
    edge_descriptor edge,
    partition_iterator piter,
    const V& new_variant,
    score_type score,
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

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
typename chart<C,S,V,R,I>::rule_type chart<C,S,V,R,I>::partition_rule(
    partition_iterator piter)
{
    return (*piter).getRuleId();
}
*/

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
int chart<C,S,V,R,I>::partition_rule_id(
    partition_iterator piter)
{
    return (*piter).getRuleId();
}

/*
template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
std::vector<typename chart<C,S,V,R,I>::edge_descriptor>& chart<C,S,V,R,I>::partition_links(
    partition_iterator piter)
{
    return (*piter).links;
}
*/

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
Lattice::Partition::Iterator chart<C,S,V,R,I>::partition_links_iterator(partition_iterator piter)
{
    return Lattice::Partition::Iterator(lattice_, *piter);
}

/*
template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
typename chart<C,S,V,R,I>::partition_iterator chart<C,S,V,R,I>::variant_partition(
    variant_iterator vit)
{
    return (*vit).pit;
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
typename param_type<V>::type chart<C,S,V,R,I>::edge_variant_category(variant_iterator vit)
{
    return (*vit).variant_category;
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
score_type chart<C,S,V,R,I>::variant_score(variant_iterator vit)
{
    return (*vit).score;
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
std::vector<typename chart<C,S,V,R,I>::variant_iterator>&
chart<C,S,V,R,I>::variant_links(variant_iterator vit)
{
    return (*vit).links;
}
*/

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
void chart<C,S,V,R,I>::sort_topologically()
{
    size_t ix = 0;
    Lattice::VertexIterator vi(lattice_);
    while (vi.hasNext()) {
        vertex_descriptor vd = vi.next();
        sorted_vertices_.push_back(vd);
        topological_order_[vd] = ix++;
    }
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
size_t chart<C,S,V,R,I>::topological_index(vertex_descriptor vertex) const
{
    assert(topological_order_.count(vertex));

    return (*(topological_order_.find(vertex))).second;
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
typename chart<C,S,V,R,I>::vertex_descriptor chart<C,S,V,R,I>::vertex_by_index(size_t i) const
{
    assert(i < sorted_vertices_.size());

    return sorted_vertices_[i];
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
size_t chart<C,S,V,R,I>::topological_count() const
{
    return sorted_vertices_.size();
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
void chart<C,S,V,R,I>::setTagMask(LayerTagMask layerTagMask) {
    tagMask_ = layerTagMask;
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
typename chart<C,S,V,R,I>::vertex_descriptor chart<C,S,V,R,I>:: getFirstVertex() const {
    return lattice_.getFirstVertex();
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
typename chart<C,S,V,R,I>::vertex_descriptor chart<C,S,V,R,I>:: getLastVertex() const {
    return lattice_.getLastVertex();
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
const LayerTagCollection& chart<C,S,V,R,I>::getGobioTag_() const {
    return gobioTag_;
}

template<typename C, typename S, typename V, typename R, template<typename, typename> class I>
const LayerTagMask& chart<C,S,V,R,I>::getTagMask_() const {
    return tagMask_;
}


#endif
