#ifndef AV_AI_CONVERTER_HDR
#define AV_AI_CONVERTER_HDR


#include "annotation_item.hpp"
#include "lattice.hpp"


class AV_AI_Converter {

public:

    AV_AI_Converter(Lattice & lattice) : lattice_(lattice) { }

    template <typename AVMatrixType>
    const AnnotationItem toAnnotationItem(AVMatrixType av);

private:

    Lattice & lattice_;

};


template <typename AVMatrixType>
const AnnotationItem AV_AI_Converter::toAnnotationItem(AVMatrixType av) {
    AnnotationItem result("dummy");
    // TODO
    return result;
}


#endif
