#ifndef EXTRACT_TREE_TPL_HDR
#define EXTRACT_TREE_TPL_HDR

#include "extract_tree.hpp"
#include "av_matrix.hpp"

#include <stack>

#ifndef PRINTRULES
#define PRINTRULES 0
#endif

#if PRINTRULES

#define _T5_WITH_EXTSCORES
#define PRINTSYMBOL(EDGE) "[" << chart.edge_category(EDGE).catprint() << ":" << combinator.get_symbol_registrar().get_obj(chart.edge_category(EDGE).get_cat()) << "]"

#include "md5.hpp"
#include <boost/archive/text_oarchive.hpp>

#endif //PRINTRULES

template<class Ch>
typename Ch::edge_descriptor edge_by_hook_path(
    Ch& chart,
    typename Ch::variant_iterator& vit,
    const std::vector<size_t>& path);

template<class T, class Ch, class K>
boost::shared_ptr<tree_branch<T,Ch,typename K::equivalent_type> > extract_tree_branch_with_spec(
    Ch& chart,
    typename Ch::edge_descriptor edge,
    typename Ch::variant_iterator vit,
    K& combinator,
    const std::vector<typename K::rule_holder>& local_rules,
    boost::shared_ptr<tree_specification<T> > spec,
    bool is_main
#if PRINTRULES
    , bool printrules = false
#endif //PRINTRULES
    );

template<class T, class Ch, class K>
boost::shared_ptr<tree_branch<T,Ch,typename K::equivalent_type> > extract_tree_branch(
    Ch& chart,
    typename Ch::edge_descriptor edge,
    K& combinator,
    const std::vector<typename K::rule_holder>& local_rules)
{
    std::pair<
    typename Ch::variant_iterator,
    typename Ch::variant_iterator> vits
    = chart.edge_variants(edge);

    if(vits.first == vits.second)
    return boost::shared_ptr<tree_branch<T,Ch,typename K::equivalent_type> >();

    typename Ch::partition_iterator pit = chart.variant_partition(vits.first);

    boost::shared_ptr<tree_specification<T> > core_spec
    = combinator.tree_spec(chart.partition_rule(pit), local_rules);

    return extract_tree_branch_with_spec<T,Ch,K>(
    chart, edge, vits.first, combinator, local_rules, core_spec, true);
}

template<class T, class  Ch>
struct extract_tree_branch_with_spec_stack_element
{
    boost::shared_ptr<tree_specification<T> > spec;
    typename Ch::edge_descriptor edge;
    typename Ch::variant_iterator variant_it;

    extract_tree_branch_with_spec_stack_element(
    boost::shared_ptr<tree_specification<T> > a_spec,
    typename Ch::edge_descriptor a_edge,
    typename Ch::variant_iterator a_variant_it):
    spec(a_spec),
    edge(a_edge),
    variant_it(a_variant_it)
    {

    }
};

template<class T, class Ch, class K>
bool add_child_or_contents_to_tree_branch(
    Ch& ch,
    boost::shared_ptr<tree_branch<T,Ch,typename K::equivalent_type> > tb,
    T label,
    boost::shared_ptr<tree_specification<T> > spec,
    typename Ch::edge_descriptor edge,
    typename Ch::variant_iterator variant_it,
    K& combinator,
    const std::vector<typename K::rule_holder>& local_rules);


template<class T, class Ch, class K>
boost::shared_ptr<tree_branch<T,Ch,typename K::equivalent_type> > extract_tree_branch_with_spec(
    Ch& chart,
    typename Ch::edge_descriptor edge,
    typename Ch::variant_iterator vit,
    K& combinator,
    const std::vector<typename K::rule_holder>& local_rules,
    boost::shared_ptr<tree_specification<T> > spec,
    bool is_main
#if PRINTRULES
    , bool printrules
#endif //PRINTRULES
    )
{
    boost::shared_ptr<tree_branch<T,Ch,typename K::equivalent_type> > tb(new tree_branch<T,Ch,typename K::equivalent_type>());

    bool support_found = false;

    if(is_main)
    {
    tb->set_support(edge, vit);
    support_found = true;
    }

    typename K::equivalent_type e = combinator.equivalent(
    chart.partition_rule(chart.variant_partition(vit)), local_rules);

    if(e)
    tb->set_equivalent(e);


    typename Ch::edge_descriptor r_edge = edge;
    typename Ch::variant_iterator r_vit  = vit;

    std::stack<extract_tree_branch_with_spec_stack_element<T, Ch> > r_stack;

    while(1)
    {

#if PRINTRULES

if (printrules) {
    stringstream sstream;
    boost::archive::text_oarchive oa(sstream);
    typename Ch::partition_iterator pi = chart.edge_partitions(r_edge).first;
    if (chart.partition_links(pi).size() > 0) {
        int intcat = chart.edge_category(r_edge).catprint();
        oa << intcat;
        std::cout << PRINTSYMBOL(r_edge) << " ->";
        for (
            typename std::vector<typename Ch::edge_descriptor>::iterator li = chart.partition_links(pi).begin();
            li != chart.partition_links(pi).end();
            ++li
        ) {
            intcat = chart.edge_category(*li).catprint();
            oa << intcat;
            std::cout << " " << PRINTSYMBOL(*li);
        }
        std::cout << " {" << md5(sstream.str() + "\n") << "}" << std::endl;
    }
}

#endif //PRINTRULES

    // odkładamy na stosie bieżącą specyfikację drzewa i partycję, żeby
    // póżniej móc dodać prawe dzieci (wykorzystujemy stos, żeby nie wywoływać
    // rekurencyjnie funkcji)
    r_stack.push(
        extract_tree_branch_with_spec_stack_element<T, Ch>(
        spec,
        r_edge,
        vit));

    // dodajemy lewe dzieci
    for(size_t li = 0; li < spec->nb_left_subspecs(); ++li)
        if(!add_child_or_contents_to_tree_branch<T,Ch,K>(
           chart,
           tb,
           spec->left_subspec_label(li),
           spec->left_subspec(li),
           r_edge,
           vit,
           combinator,
           local_rules))
        return boost::shared_ptr<tree_branch<T,Ch,typename K::equivalent_type> >();

    // sprawdzamy, czy doszliśmy do końca
    if(spec->root()->is_final())
    {
        tb->set_root(spec->root()->get_category(combinator.get_master().false_value()));
        break;
    }

    assert(spec->root()->is_hook());

    r_edge = edge_by_hook_path(chart, vit, spec->root()->get_hook_path());

    spec = combinator.tree_spec(
        chart.partition_rule(chart.variant_partition(vit)), local_rules);

    if(!support_found)
    {
        tb->set_support(r_edge, vit);
        support_found = true;
    }

    if(!tb->equivalent())
    {
        typename K::equivalent_type e = combinator.equivalent(
        chart.partition_rule(chart.variant_partition(vit)), local_rules);

        if(e)
        tb->set_equivalent(e);
    }
    }

    // dodajemy prawe dzieci zgodnie ze specyfikacjami (które wcześniej
    // odłożyliśmy na stosie)
    while(!r_stack.empty())
    {
    for(size_t ri = 0; ri < r_stack.top().spec->nb_right_subspecs(); ++ri)
        if(!add_child_or_contents_to_tree_branch<T,Ch,K>(
           chart,
           tb,
           r_stack.top().spec->right_subspec_label(ri),
           r_stack.top().spec->right_subspec(ri),
           r_stack.top().edge,
           r_stack.top().variant_it,
           combinator,
           local_rules))
        return boost::shared_ptr<tree_branch<T,Ch,typename K::equivalent_type> >();

    r_stack.pop();
    }

    return tb;
}

template<class T, class Ch, class K>
bool add_child_or_contents_to_tree_branch(
    Ch& chart,
    boost::shared_ptr<tree_branch<T,Ch,typename K::equivalent_type> > tb,
    T label,
    boost::shared_ptr<tree_specification<T> > spec,
    typename Ch::edge_descriptor edge,
    typename Ch::variant_iterator variant_it,
    K& combinator,
    const std::vector<typename K::rule_holder>& local_rules)
{
    if(spec->root()->is_contents_hook())
    {
    typename Ch::variant_iterator r_vit = variant_it;

    typename Ch::edge_descriptor r_edge
        = edge_by_hook_path<Ch>(
        chart, r_vit, spec->root()->get_hook_path());

    boost::shared_ptr<tree_branch<T,Ch,typename K::equivalent_type> > r_tb
        = extract_tree_branch<T,Ch,K>(
        chart,
        r_edge,
        combinator,
        local_rules);

    if(!r_tb)
        return false;

    assert(tb != r_tb);

    for(size_t i = 0; i < r_tb->nb_children(); ++i)
        tb->add_child(
        r_tb->child_label(i),
        r_tb->child_spec(i),
        r_tb->child_edge(i),
        r_tb->child_variant_it(i));
    }
    else
    {
    tb->add_child(
        label,
        spec,
        edge,
        variant_it);
    }

    return true;
}


template<class Ch>
typename Ch::edge_descriptor edge_by_hook_path(
    Ch& chart,
    typename Ch::variant_iterator& vit,
    const std::vector<size_t>& path)
{
    assert(!path.empty());

    typename Ch::partition_iterator ppit = chart.variant_partition(vit);
    typename Ch::edge_descriptor e;

    for(std::vector<size_t>::const_iterator it = path.begin();
    it != path.end();
    ++it)
    {
    assert(*it < chart.partition_links(ppit).size());
    assert(chart.partition_links(ppit).size()
           == chart.variant_links(vit).size());

    e = (chart.partition_links(ppit))[*it];
    vit = (chart.variant_links(vit))[*it];

    ppit = chart.variant_partition(vit);
    }

    return e;
}

template<class T, class Ch, class K>
void show_tree_with_spec(
    Ch& chart,
    typename Ch::edge_descriptor edge,
    typename Ch::variant_iterator vit,
    K& combinator,
    const std::vector<typename K::rule_holder>& local_rules,
    std::ostream& ostr,
    const std::string& indent,
    boost::shared_ptr<tree_specification<T> > spec,
    T label,
    bool is_main);

template<class T, class Ch, class K>
void show_tree(
    Ch& chart,
    typename Ch::edge_descriptor edge,
    K& combinator,
    const std::vector<typename K::rule_holder>& local_rules,
    std::ostream& ostr)
{
    std::pair<
    typename Ch::variant_iterator,
    typename Ch::variant_iterator> vits
    = chart.edge_variants(edge);

    if(vits.first == vits.second)
    {
    ostr << "(NULL)\n";
    return;
    }

    typename Ch::partition_iterator pit = chart.variant_partition(vits.first);

    boost::shared_ptr<tree_specification<T> > core_spec
    = combinator.tree_spec(chart.partition_rule(pit), local_rules);

    assert(core_spec);

    show_tree_with_spec<T,Ch,K>(
    chart,
    edge,
    vits.first,
    combinator,
    local_rules,
    ostr,
    std::string(""),
    core_spec,
    combinator.get_master().false_value(),
    true);
}

template<class T, class Ch, class K>
void show_tree_with_spec(
    Ch& chart,
    typename Ch::edge_descriptor edge,
    typename Ch::variant_iterator vit,
    K& combinator,
    const std::vector<typename K::rule_holder>& local_rules,
    std::ostream& ostr,
    const std::string& indent,
    boost::shared_ptr<tree_specification<T> > spec,
    T label,
    bool is_main)
{
    boost::shared_ptr<tree_branch<T,Ch,typename K::equivalent_type> > tb = extract_tree_branch_with_spec(
    chart, edge, vit, combinator, local_rules, spec, is_main);

    ostr << indent << combinator.get_master().string_representation(tb->root());

    if(combinator.get_master().is_true(label))
    ostr << ':' << combinator.get_master().string_representation(label);

    if(tb->is_supported())
    {
    const av_matrix<int, T>& avm = chart.edge_category(tb->supporting_edge());
    const typename bare_av_matrix<T>::type& v_avm
        = chart.edge_variant_category(tb->supporting_variant());
    T def = combinator.get_master().false_value();

    ostr << "\t\t";

    for(int ai = 0; ai < avm.nb_attrs(); ++ai)
        if(combinator.get_master().is_true(avm.get_attr(ai, def)))
        ostr << ' '
             << combinator.get_attribute_registrar().get_obj(ai)
             << '='
             << combinator.get_master().string_representation(
             avm.get_attr(ai, def));

    ostr << " || ";
    for(int v_ai = 0; v_ai < v_avm.nb_attrs(); ++v_ai)
        if(combinator.get_master().is_true(v_avm.get_attr(v_ai, def)))
        ostr << ' '
             << combinator.get_extra_attribute_registrar().get_obj(v_ai)
             << '='
             << combinator.get_master().string_representation(
             v_avm.get_attr(v_ai, def));
    }

    ostr << std::endl;

    for(size_t i = 0; i < tb->nb_children(); ++i)
    show_tree_with_spec<T,Ch,K>(
        chart,
        tb->child_edge(i),
        tb->child_variant_it(i),
        combinator,
        local_rules,
        ostr,
        indent + std::string("  "),
        tb->child_spec(i),
        tb->child_label(i),
        false);
}


#endif
