#include "semantics_wrapper.hpp"


template<typename Atom, typename Master>
semantics_wrapper<Atom, Master>::semantics_wrapper(
    Master & master, int bonus_divisor
) :
    master_(master),
    bonus_divisor_(bonus_divisor)
{ }


template<typename Atom, typename Master>
void semantics_wrapper<Atom, Master>::set_divisor(int bonus_divisor) {
    bonus_divisor_ = bonus_divisor;
}


template<typename Atom, typename Master>
Atom semantics_wrapper<Atom, Master>::intersect(
    Atom sem,
    Atom pattern,
    double & bonus
) {
    return tri_intersect(master_.false_value(), sem, pattern, bonus);
}


template<typename Atom, typename Master>
Atom semantics_wrapper<Atom, Master>::tri_intersect(
    Atom word,
    Atom sem,
    Atom pattern,
    double & bonus
) {
    Atom ar = tri_intersect_(word, sem, pattern, bonus);
    bonus /= bonus_divisor_;
    return ar;
}


template<typename Atom, typename Master>
Atom semantics_wrapper<Atom, Master>::tri_intersect_(
    Atom /*word*/,
    Atom sem,
    Atom pattern,
    double & bonus
) {
    bonus = 0.0;

    if (master_.is_false(pattern) || master_.is_false(sem)) {
        return master_.false_value();
    }

    if (master_.is_any(pattern)) {
        return master_.any_value();
    }

    std::string pattern_s = master_.string_representation(pattern);
    std::string sem_s = master_.string_representation(sem);

    if (master_.is_any(sem) && strchr(pattern_s.c_str(), '\'') == 0) {
        if (strchr(pattern_s.c_str(), '[') == 0) {
            bonus = 1;
            return master_.any_value();
        } else {
            return master_.false_value();
        }
    }

    return master_.false_value();
}

