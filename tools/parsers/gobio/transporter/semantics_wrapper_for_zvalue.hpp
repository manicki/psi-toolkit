#ifndef SEMANTICS_WRAPPER_FOR_ZVALUE_HDR
#define SEMANTICS_WRAPPER_FOR_ZVALUE_HDR


#include "semantics_wrapper.hpp"
#include "zvalue.hpp"
#include "zvalue_master.hpp"


class semantics_wrapper_for_zvalue {

public:
    semantics_wrapper_for_zvalue(zvalue_master&, int bonus_divisor=1);

    zvalue intersect(zvalue sem, zvalue pattern, double& bonus);
    zvalue tri_intersect(zvalue word, zvalue sem, zvalue pattern, double& bonus);

    void set_divisor(int bonus_divisor);

private:
    semantics_wrapper semantics_wrapper_;

};


#endif
