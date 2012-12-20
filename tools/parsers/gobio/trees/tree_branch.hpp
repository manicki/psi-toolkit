#ifndef TREE_BRANCH_HPP_HDR
#define TREE_BRANCH_HPP_HDR

#include <boost/shared_ptr.hpp>
#include <vector>

#include "tree_specification.hpp"

//! Rozgalezienie drzewa skladniowego
/**
 * Rozgalezienie to korzen, dodatkowe dane (zwykle atrybuty) i
 * ciag ugruntowanych specyfikacji. Z kazdej takiej krawedzi mozna
 * wygenerowac jedno poddrzewo skladniowe. Dodatkowo
 * krawedzie opatrzone sa etykietami.
 *
 * Rozgalezienie nie jest calym drzewem, bowiem nie zawiera - bezposrednio
 * - rekurencyjnie poddrzew. Poddrzewa musza zostac wygenerowane na podstawie
 * odpowiednich krawedzi chartu.
 *
 * \arg T - typ kategorii i etykiet w drzewie skladniowym
 * \arg Ch - typ chartu
 */
template<class T, class Ch, class E>
class tree_branch
{
public:
    tree_branch();

    //! Zwraca kategorie korzenia
    T root() const;
    //! Ustawia kategorie korzenia
    void set_root(T a_root);

    typename Ch::edge_descriptor supporting_edge();
    typename Ch::variant_iterator supporting_variant();
    bool is_supported();
    void set_support(
    typename Ch::edge_descriptor edge,
    typename Ch::variant_iterator vit);

    void set_equivalent(E eq);
    E equivalent();

    //! Zwraca liczbe poddrzew
    size_t nb_children() const;
    //! Zwraca etykiete poddrzewa \a i
    T child_label(size_t i) const;
    //! Zwraca krawedz chartu, z ktorego mozna wygenerowac poddrzewo \a i
    boost::shared_ptr<tree_specification<T> > child_spec(size_t i);

    typename Ch::variant_iterator child_variant_it(size_t i);

    typename Ch::edge_descriptor child_edge(size_t i);

    //! Dodaj kolejne poddrzewo
    void add_child(
    T label,
    boost::shared_ptr<tree_specification<T> > spec,
    typename Ch::edge_descriptor edge,
    typename Ch::variant_iterator variant_it);

private:
    T root_;
    typename Ch::edge_descriptor edge_;
    typename Ch::variant_iterator variant_it_;
    bool is_supported_;

    struct child_link
    {
    T label;
    boost::shared_ptr<tree_specification<T> > spec;
    typename Ch::edge_descriptor edge;
    typename Ch::variant_iterator variant_it;

    child_link(
        T a_label,
        boost::shared_ptr<tree_specification<T> > a_spec,
        typename Ch::edge_descriptor a_edge,
        typename Ch::variant_iterator a_variant_it):
        label(a_label),
        spec(a_spec),
        edge(a_edge),
        variant_it(a_variant_it) {}
    };

    std::vector<child_link> children_;

    E equivalent_;
};

#endif
