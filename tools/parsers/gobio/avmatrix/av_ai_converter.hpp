#ifndef AV_AI_CONVERTER_HDR
#define AV_AI_CONVERTER_HDR


#include <string>

#include "annotation_item.hpp"
#include "av_matrix.hpp"
#include "lattice.hpp"


class AV_AI_Converter {

public:

    AV_AI_Converter(Lattice & lattice) : lattice_(lattice) { }

    const AnnotationItem toAnnotationItem(av_matrix<int, int> av);

    const AnnotationItem toAnnotationItem(std::string av);

private:

    Lattice & lattice_;

};


#endif
