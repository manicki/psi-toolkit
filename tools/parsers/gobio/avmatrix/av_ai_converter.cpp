#include "av_ai_converter.hpp"


#include <sstream>


const AnnotationItem AV_AI_Converter::toAnnotationItem(av_matrix<int, int> av) {
    std::stringstream catSs;
    std::stringstream attrSs;
    std::stringstream valSs;
    catSs << av.get_cat();
    AnnotationItem result(catSs.str());
    for (int i = 0; i < av.nb_attrs(); ++i) {
        attrSs << i;
        valSs << av.get_attr(i);
        lattice_.getAnnotationItemManager().setValue(result, attrSs.str(), valSs.str());
        attrSs.str("");
        valSs.str("");
    }
    return result;
}


const AnnotationItem AV_AI_Converter::toAnnotationItem(std::string av) {
    AnnotationItem result(av);
    return result;
}
