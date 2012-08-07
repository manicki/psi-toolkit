#ifndef AV_AI_CONVERTER_HDR
#define AV_AI_CONVERTER_HDR


#include <cctype>
#include <sstream>
#include <string>

#include <boost/foreach.hpp>

#include "annotation_item.hpp"
#include "annotation_item_manager.hpp"
#include "av_matrix.hpp"
#include "lattice.hpp"
#include "number_master.hpp"
#include "registrar.tpl"
#include "zvalue.hpp"


namespace AV_AI_Converter_specialization {

    template <typename CategoryType>
    const CategoryType toAVMatrix(
        AnnotationItem ai,
        Lattice & /*lattice*/,
        registrar<std::string> & /* symbol_reg */,
        registrar<std::string> & /* attribute_reg */,
        bool /* convert_cases */
    ) {
        return (CategoryType)(ai.getCategory());
    }

    template <>
    inline const av_matrix<int, int> toAVMatrix< av_matrix<int, int> >(
        AnnotationItem ai,
        Lattice & lattice,
        registrar<std::string> & symbol_reg,
        registrar<std::string> & attribute_reg,
        bool convert_cases
    ) {
        std::string category = ai.getCategory();
        if (convert_cases && !category.empty()) category[0] = tolower(category[0]);
        av_matrix<int, int> result(symbol_reg.get_id(category));
        number_master master;
        typedef std::pair<std::string, std::string> StringPair;
        std::list< StringPair > values
            = lattice.getAnnotationItemManager().getValues(ai);
        BOOST_FOREACH( StringPair avpair, values ) {
            std::string attr = avpair.first;
            if (convert_cases && !attr.empty()) attr[0] = toupper(attr[0]);
            std::stringstream valSs(avpair.second);
            int v;
            valSs >> v;
            result.set_attr(attribute_reg.get_id(attr), v, master.false_value());
        }
        return result;
    }

    template <>
    inline const av_matrix<int, zvalue> toAVMatrix< av_matrix<int, zvalue> >(
        AnnotationItem ai,
        Lattice & lattice,
        registrar<std::string> & symbol_reg,
        registrar<std::string> & attribute_reg,
        bool convert_cases
    ) {
        std::string category = ai.getCategory();
        if (convert_cases && !category.empty()) category[0] = tolower(category[0]);
        av_matrix<int, zvalue> result(symbol_reg.get_id(category));
        AnnotationItemManager & aim = lattice.getAnnotationItemManager();
        typedef std::pair<std::string, zvalue> StringZvaluePair;
        std::list< StringZvaluePair > values = aim.getValuesAsZvalues(ai);
        BOOST_FOREACH( StringZvaluePair avpair, values ) {
            std::string attr = avpair.first;
            if (convert_cases && !attr.empty()) attr[0] = toupper(attr[0]);
            result.set_attr(attribute_reg.get_id(attr), avpair.second, aim.false_value());
        }
        return result;
    }

    template <>
    inline const av_matrix<std::string, int> toAVMatrix< av_matrix<std::string, int> >(
        AnnotationItem ai,
        Lattice & lattice,
        registrar<std::string> & /* symbol_reg */,
        registrar<std::string> & attribute_reg,
        bool convert_cases
    ) {
        std::string category = ai.getCategory();
        if (convert_cases && !category.empty()) category[0] = tolower(category[0]);
        av_matrix<std::string, int> result(category);
        number_master master;
        typedef std::pair<std::string, std::string> StringPair;
        std::list< StringPair > values
            = lattice.getAnnotationItemManager().getValues(ai);
        BOOST_FOREACH( StringPair avpair, values ) {
            std::string attr = avpair.first;
            if (convert_cases && !attr.empty()) attr[0] = toupper(attr[0]);
            std::stringstream valSs(avpair.second);
            int v;
            valSs >> v;
            result.set_attr(attribute_reg.get_id(attr), v, master.false_value());
        }
        return result;
    }

    template <>
    inline const av_matrix<std::string, zvalue> toAVMatrix< av_matrix<std::string, zvalue> >(
        AnnotationItem ai,
        Lattice & lattice,
        registrar<std::string> & /* symbol_reg */,
        registrar<std::string> & attribute_reg,
        bool convert_cases
    ) {
        std::string category = ai.getCategory();
        if (convert_cases && !category.empty()) category[0] = tolower(category[0]);
        av_matrix<std::string, zvalue> result(category);
        AnnotationItemManager & aim = lattice.getAnnotationItemManager();
        typedef std::pair<std::string, zvalue> StringZvaluePair;
        std::list< StringZvaluePair > values = aim.getValuesAsZvalues(ai);
        BOOST_FOREACH( StringZvaluePair avpair, values ) {
            std::string attr = avpair.first;
            if (convert_cases && !attr.empty()) attr[0] = toupper(attr[0]);
            result.set_attr(attribute_reg.get_id(attr), avpair.second, aim.false_value());
        }
        return result;
    }

    template <>
    inline const std::string toAVMatrix< std::string >(
        AnnotationItem ai,
        Lattice & /* lattice */,
        registrar<std::string> & /* symbol_reg */,
        registrar<std::string> & /* attribute_reg */,
        bool convert_cases
    ) {
        std::string category = ai.getCategory();
        if (convert_cases && !category.empty()) category[0] = tolower(category[0]);
        return category;
    }

}


class AV_AI_Converter {

public:


    /**
     * convert_cases :
     *   Gobio grammar files require all categories to begin with a lowercase letter
     *   and all attributes to begin with an uppercase letter.
     *   If convert_cases is switched on, such case conversion is performed
     *   while converting from AV to AI.
     */
    AV_AI_Converter(
        Lattice & lattice,
        registrar<std::string> & symbol_reg,
        registrar<std::string> & attribute_reg,
        bool convert_cases = false
    ) :
        lattice_(lattice),
        symbol_reg_(symbol_reg),
        attribute_reg_(attribute_reg),
        convert_cases_(convert_cases)
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
    bool convert_cases_;

};


template <typename CategoryType>
const CategoryType AV_AI_Converter::toAVMatrix(AnnotationItem ai) {
    return AV_AI_Converter_specialization::toAVMatrix<CategoryType>(
        ai,
        lattice_,
        symbol_reg_,
        attribute_reg_,
        convert_cases_
    );
}


#endif
