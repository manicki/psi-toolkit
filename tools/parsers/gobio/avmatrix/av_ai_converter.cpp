#include "av_ai_converter.hpp"


const AnnotationItem AV_AI_Converter::toAnnotationItem(av_matrix<int, int> av) {
    std::stringstream attrSs;
    std::stringstream valSs;
    std::stringstream catSs;
    int cat = av.get_cat();
    if (cat > symbol_reg_.last_id()) {
        catSs << cat;
    } else {
        catSs << symbol_reg_.get_obj(cat);
    }
    AnnotationItem result(catSs.str());
    for (int i = 0; i < av.nb_attrs(); ++i) {
        if (i > attribute_reg_.last_id()) {
            attrSs << i;
        } else {
            attrSs << symbol_reg_.get_obj(i);
        }
        valSs << av.get_attr(i);
        lattice_.getAnnotationItemManager().setValue(result, attrSs.str(), valSs.str());
        attrSs.str("");
        valSs.str("");
    }
    return result;
}


const AnnotationItem AV_AI_Converter::toAnnotationItem(av_matrix<int, zvalue> av) {
    std::stringstream attrSs;
    std::stringstream catSs;
    int cat = av.get_cat();
    if (cat > symbol_reg_.last_id()) {
        catSs << cat;
    } else {
        catSs << symbol_reg_.get_obj(cat);
    }
    AnnotationItem result(catSs.str());
    for (int i = 0; i < av.nb_attrs(); ++i) {
        if (i > attribute_reg_.last_id()) {
            attrSs << i;
        } else {
            attrSs << symbol_reg_.get_obj(i);
        }
        lattice_.getAnnotationItemManager().setValue(result, attrSs.str(), av.get_attr(i));
        attrSs.str("");
    }
    return result;
}


const AnnotationItem AV_AI_Converter::toAnnotationItem(av_matrix<std::string, int> av) {
    std::stringstream attrSs;
    std::stringstream valSs;
    AnnotationItem result(av.get_cat());
    for (int i = 0; i < av.nb_attrs(); ++i) {
        attrSs << i;
        valSs << av.get_attr(i);
        lattice_.getAnnotationItemManager().setValue(result, attrSs.str(), valSs.str());
        attrSs.str("");
        valSs.str("");
    }
    return result;
}


const AnnotationItem AV_AI_Converter::toAnnotationItem(av_matrix<std::string, zvalue> av) {
    std::stringstream attrSs;
    AnnotationItem result(av.get_cat());
    for (int i = 0; i < av.nb_attrs(); ++i) {
        attrSs << i;
        lattice_.getAnnotationItemManager().setValue(result, attrSs.str(), av.get_attr(i));
        attrSs.str("");
    }
    return result;
}


const AnnotationItem AV_AI_Converter::toAnnotationItem(std::string av) {
    AnnotationItem result(av);
    return result;
}
