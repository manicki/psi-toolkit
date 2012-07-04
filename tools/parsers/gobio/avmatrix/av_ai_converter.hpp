#ifndef AV_AI_CONVERTER_HDR
#define AV_AI_CONVERTER_HDR


#include <sstream>
#include <string>

#include <boost/foreach.hpp>

#include "annotation_item.hpp"
#include "av_matrix.hpp"
#include "lattice.hpp"
#include "number_master.hpp"
#include "registrar.tpl"
#include "zvalue.hpp"
#include "zvalue_master.hpp"


namespace AV_AI_Converter_specialization {

    template <typename CategoryType>
    const CategoryType toAVMatrix(
        AnnotationItem ai,
        Lattice & /*lattice*/,
        registrar<std::string> & /* symbol_reg */
    ) {
        return (CategoryType)(ai.getCategory());
    }

    template <>
    inline const av_matrix<int, int> toAVMatrix< av_matrix<int, int> >(
        AnnotationItem ai,
        Lattice & lattice,
        registrar<std::string> & symbol_reg
    ) {
        // int category;
        // std::stringstream catSs(ai.getCategory());
        // catSs >> category;
        // av_matrix<int, int> result(category);
        av_matrix<int, int> result(symbol_reg.get_id(ai.getCategory()));
        number_master master;
        typedef std::pair<std::string, std::string> StringPair;
        std::list< StringPair > values
            = lattice.getAnnotationItemManager().getValues(ai);
        BOOST_FOREACH( StringPair avpair, values ) {
            std::stringstream valSs(avpair.second);
            int v;
            valSs >> v;
            result.set_attr(symbol_reg.get_id(avpair.first), v, master.false_value());
        }
        return result;
    }

    template <>
    inline const av_matrix<int, zvalue> toAVMatrix< av_matrix<int, zvalue> >(
        AnnotationItem ai,
        Lattice & lattice,
        registrar<std::string> & symbol_reg
    ) {
        // int category;
        // std::stringstream catSs(ai.getCategory());
        // catSs >> category;
        // av_matrix<int, zvalue> result(category);
        av_matrix<int, zvalue> result(symbol_reg.get_id(ai.getCategory()));
        zvalue_master master;
        typedef std::pair<std::string, zvalue> StringZvaluePair;
        std::list< StringZvaluePair > values
            = lattice.getAnnotationItemManager().getValuesAsZvalues(ai);
        BOOST_FOREACH( StringZvaluePair avpair, values ) {
            result.set_attr(symbol_reg.get_id(avpair.first), avpair.second, master.false_value());
        }
        return result;
    }

    template <>
    inline const av_matrix<std::string, int> toAVMatrix< av_matrix<std::string, int> >(
        AnnotationItem ai,
        Lattice & lattice,
        registrar<std::string> & /* symbol_reg */
    ) {
        av_matrix<std::string, int> result(ai.getCategory());
        number_master master;
        typedef std::pair<std::string, std::string> StringPair;
        std::list< StringPair > values
            = lattice.getAnnotationItemManager().getValues(ai);
        BOOST_FOREACH( StringPair avpair, values ) {
            std::stringstream attrSs(avpair.first);
            std::stringstream valSs(avpair.second);
            int a;
            int v;
            attrSs >> a;
            valSs >> v;
            result.set_attr(a, v, master.false_value());
        }
        return result;
    }

    template <>
    inline const av_matrix<std::string, zvalue> toAVMatrix< av_matrix<std::string, zvalue> >(
        AnnotationItem ai,
        Lattice & lattice,
        registrar<std::string> & /* symbol_reg */
    ) {
        av_matrix<std::string, zvalue> result(ai.getCategory());
        zvalue_master master;
        typedef std::pair<std::string, zvalue> StringZvaluePair;
        std::list< StringZvaluePair > values
            = lattice.getAnnotationItemManager().getValuesAsZvalues(ai);
        BOOST_FOREACH( StringZvaluePair avpair, values ) {
            std::stringstream attrSs(avpair.first);
            int a;
            attrSs >> a;
            result.set_attr(a, avpair.second, master.false_value());
        }
        return result;
    }

    template <>
    inline const std::string toAVMatrix< std::string >(
        AnnotationItem ai,
        Lattice & /* lattice */,
        registrar<std::string> & /* symbol_reg */
    ) {
        return ai.getCategory();
    }

}


class AV_AI_Converter {

public:

    AV_AI_Converter(
        Lattice & lattice,
        registrar<std::string> & symbol_reg,
        registrar<std::string> & attribute_reg
    ) :
        lattice_(lattice),
        symbol_reg_(symbol_reg),
        attribute_reg_(attribute_reg)
    { }

    const AnnotationItem toAnnotationItem(av_matrix<int, int> av);

    const AnnotationItem toAnnotationItem(av_matrix<int, zvalue> av);

    const AnnotationItem toAnnotationItem(av_matrix<std::string, int> av);

    const AnnotationItem toAnnotationItem(av_matrix<std::string, zvalue> av);

    const AnnotationItem toAnnotationItem(std::string av);

    template <typename CategoryType>
    const CategoryType toAVMatrix(AnnotationItem ai);

private:

    Lattice & lattice_;
    registrar<std::string> & symbol_reg_;
    registrar<std::string> & attribute_reg_;

};


template <typename CategoryType>
const CategoryType AV_AI_Converter::toAVMatrix(AnnotationItem ai) {
    return AV_AI_Converter_specialization::toAVMatrix<CategoryType>(ai, lattice_, symbol_reg_);
}


#endif
