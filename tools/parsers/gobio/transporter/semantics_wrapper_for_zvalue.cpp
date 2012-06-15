#include "semantics_wrapper_for_zvalue.hpp"


semantics_wrapper_for_zvalue::semantics_wrapper_for_zvalue(
    zvalue_master& master,
    int bonus_divisor
) :
    semantics_wrapper_(master.get_number_master(), bonus_divisor)
{ }


void semantics_wrapper_for_zvalue::set_divisor(int bonus_divisor) {
    semantics_wrapper_.set_divisor(bonus_divisor);
}


zvalue semantics_wrapper_for_zvalue::intersect(
    zvalue sem,
    zvalue pattern,
    double& bonus
) {
    return INTEGER_TO_ZVALUE(semantics_wrapper_.intersect(
        ZVALUE_TO_INTEGER(sem),
        ZVALUE_TO_INTEGER(pattern),
        bonus
    ));
}


zvalue semantics_wrapper_for_zvalue::tri_intersect(
    zvalue word,
    zvalue sem,
    zvalue pattern,
    double& bonus
) {
    return INTEGER_TO_ZVALUE(semantics_wrapper_.tri_intersect(
        ZVALUE_TO_INTEGER(word),
        ZVALUE_TO_INTEGER(sem),
        ZVALUE_TO_INTEGER(pattern),
        bonus
    ));
}
