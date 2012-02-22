#ifndef SEMANTICS_WRAPPER_HPP_HDR
#define SEMANTICS_WRAPPER_HPP_HDR

// #include "dhierarchy.h"
#include "number_master.hpp"

class semantics_wrapper
{

public:
    semantics_wrapper(number_master&, int bonus_divisor=1);

    int intersect(int sem, int pattern, double& bonus);
    int tri_intersect(int word, int sem, int pattern, double& bonus);

    // void set_dhierarchy(DomainHierarchy* dhierarchy);
    void set_divisor(int bonus_divisor);

private:
    int tri_intersect_(int word, int sem, int pattern, double& bonus);

    number_master& master_;
    // DomainHierarchy* dhierarchy_;
    double bonus_divisor_;
};

#endif
