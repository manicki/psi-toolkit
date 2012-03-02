#ifndef AV_AI_CONVERTER_HDR
#define AV_AI_CONVERTER_HDR


#include <sstream>
#include <string>

#include <boost/foreach.hpp>

#include "annotation_item.hpp"
#include "av_matrix.hpp"
#include "lattice.hpp"


namespace AV_AI_Converter_specialization {

    template <typename CategoryType>
    const CategoryType toAVMatrix(
        Lattice & lattice,
        AnnotationItem ai
    ) {
        return (CategoryType)(ai.getCategory());
    }

    // MOŻE WYDZIELIĆ PONIŻSZE DO .tpl ???

    template <>
    inline const av_matrix<int, int> toAVMatrix< av_matrix<int, int> >(
        Lattice & lattice,
        AnnotationItem ai
    ) {
        av_matrix<int, int> result(ai.getCategory());
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
            result.set_attr(a, v, v);
        }
        return result;
    }

    template <>
    inline const std::string toAVMatrix< std::string >(
        Lattice & /* lattice */,
        AnnotationItem ai
    ) {
        return ai.getCategory();
    }

}


class AV_AI_Converter {

public:

    AV_AI_Converter(Lattice & lattice) : lattice_(lattice) { }

    const AnnotationItem toAnnotationItem(av_matrix<int, int> av);

    const AnnotationItem toAnnotationItem(std::string av);

    template <typename CategoryType>
    const CategoryType toAVMatrix(AnnotationItem ai);

private:

    Lattice & lattice_;

};


template <typename CategoryType>
const CategoryType AV_AI_Converter::toAVMatrix(AnnotationItem ai) {
    return AV_AI_Converter_specialization::toAVMatrix<CategoryType>(lattice_, ai);
}


#endif
