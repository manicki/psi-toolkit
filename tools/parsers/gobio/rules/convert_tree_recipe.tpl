#ifndef CONVERT_TREE_RECIPE_TPL_HDR
#define CONVERT_TREE_RECIPE_TPL_HDR

#include "convert_tree_recipe.hpp"

#include <boost/shared_ptr.hpp>

template<class T, class M>
boost::shared_ptr<tree_specification<T> > convert_tree_recipe(
    const GRuleTreeRecipe* recipe,
    HashWrapper<std::pair<int, int>, int>::type& rhs_hash,
    M& master,
    registrar<std::string>& symbol_registrar,
    int starred_ix)
{
    assert(recipe);

    boost::shared_ptr<tree_specification_root<T> > root;

    bool is_category_root = false;

    if (recipe->category)
    {
    root.reset(new category_root<T>(master.from_string(*(recipe->category))));
    is_category_root = true;
    }
    else if (recipe->symbol)
    {
    assert(recipe->number >= 0);

    std::pair<int, int> k(
        symbol_registrar.get_id(*(recipe->symbol)),
        recipe->number);

    assert(rhs_hash.count(k));

    if (recipe->is_contents_ref)
        root.reset(new hook_contents_root<T>(rhs_hash[k]));
    else
        root.reset(new hook_root<T>(rhs_hash[k]));
    }

    assert(root);

    boost::shared_ptr<tree_specification<T> > spec(new tree_specification<T>(root));

    if (recipe->subrecipes)
    {
    bool is_single_subrecipe = (recipe->subrecipes->size() == 1);

    for (std::list<GRuleTreeRecipe*>::const_iterator it = recipe->subrecipes->begin();
        it != recipe->subrecipes->end();
        ++it)
    {
        assert((*it));

        boost::shared_ptr<tree_specification<T> > sub_spec =
        convert_tree_recipe<T,M>(
            (*it),
            rhs_hash,
            master,
            symbol_registrar,
            starred_ix);

        assert(sub_spec->root());

        T sub_label =
        (((*it)->label && (*it)->label->length() > 0)
         ? master.from_string(*((*it)->label))
         : (((sub_spec->root() && sub_spec->root()->is_hook(starred_ix))
             ||
             // jesli mamy do czynienia z instrukcja typu A[B[...]]
             // B traktujemy domyslnie jako glowe A
             (is_category_root
             && is_single_subrecipe
             && (*it)->category))
            ? master.from_string("#")
            : master.false_value()));

        switch((*it)->how_to_insert)
        {
        case GRuleTreeRecipe::INSERT_BEFORE:
        spec->add_left_subspec(sub_spec, sub_label);
        break;
        case GRuleTreeRecipe::INSERT_AFTER:
        case GRuleTreeRecipe::ONLY_PUT:
        spec->add_right_subspec(sub_spec, sub_label);
        break;
        default:
        assert(0);
        };
    }
    }

    return spec;
}

#endif
