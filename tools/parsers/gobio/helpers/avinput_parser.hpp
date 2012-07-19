#ifndef AVINPUT_PARSER_HPP_HDR
#define AVINPUT_PARSER_HPP_HDR


#include <string>
#include <vector>

#include <boost/spirit/home/classic/tree/common.hpp>

#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_parse_tree.hpp>
#include <boost/spirit/include/classic_ast.hpp>

#include "registrar.tpl"
#include "av_matrix.hpp"
#include "chart.hpp"


template<class T>
struct simple_converter
{
private:
    registrar<std::string>& symbol_reg_;
    registrar<std::string>& attribute_reg_;
    registrar<std::string>& extra_attribute_reg_;
public:
    simple_converter(
    registrar<std::string>& symbol_reg,
    registrar<std::string>& attribute_reg,
    registrar<std::string>& extra_attribute_reg):
    symbol_reg_(symbol_reg),
    attribute_reg_(attribute_reg),
    extra_attribute_reg_(extra_attribute_reg) {}

    av_matrix<int,T>& matrix_reader(av_matrix<int,T>& x) { return x;}

    int symbol_reader(std::string& s) { return symbol_reg_.get_id(s);}
    int attribute_reader(std::string& s) { return attribute_reg_.get_id(s);}
    int extra_attribute_reader(std::string& s) { return extra_attribute_reg_.get_id(s);}
};

template<class C, class R, class T, class M, class H, class Q, class L>
class avinput_parser
{
public:
    avinput_parser(
    M& a_master,
    H& a_chart,
    Q& a_converter,
    std::vector<L>& a_local_rules):
    master_(a_master),
    chart_(a_chart),
    converter_(a_converter),
    local_rules_(a_local_rules) {}

    bool parse(const std::string& s);

    typedef char const* iterator_t;
    typedef boost::spirit::classic::tree_match<iterator_t> parse_tree_match_t;
    typedef parse_tree_match_t::tree_iterator iter_t;

private:
    M& master_;
    H& chart_;
    Q& converter_;
    std::vector<L>& local_rules_;

    std::vector<typename H::vertex_descriptor> vertices_;
    typename H::vertex_descriptor get_vertex_(int vertex_ix);

    void walk_all_(iter_t const& i);
    void walk_segment_(iter_t const& i);
    void walk_node_(
    iter_t const& i,
    typename H::vertex_descriptor start_d,
    typename H::vertex_descriptor stop_d);
    void walk_assignment_(iter_t const& i, av_matrix<C,T>& avm);
    double walk_extra_(iter_t const& i, typename bare_av_matrix<T>::type& avm);
    void walk_extra_assignment_(iter_t const& i, typename bare_av_matrix<T>::type& avm);

};


#endif

