#include "semantics_wrapper.hpp"

#include <iostream>

// jest problem z master_
semantics_wrapper::semantics_wrapper(
    number_master& master, int bonus_divisor):
    master_(master),
    // dhierarchy_(0),
    bonus_divisor_(bonus_divisor)
{
}
/*
void semantics_wrapper::set_dhierarchy(DomainHierarchy* dhierarchy)
{
    dhierarchy_ = dhierarchy;
}
*/
void semantics_wrapper::set_divisor(int bonus_divisor)
{
    bonus_divisor_ = bonus_divisor;
}

int semantics_wrapper::intersect(int sem, int pattern, double& bonus)
{
    return tri_intersect(master_.false_value(), sem, pattern, bonus);
}

int semantics_wrapper::tri_intersect(int word, int sem, int pattern, double& bonus)
{
    int ar = tri_intersect_(word, sem, pattern, bonus);

    bonus /= bonus_divisor_;

    return ar;
}

int semantics_wrapper::tri_intersect_(int /*word*/, int sem, int pattern, double& bonus)
{
    // assert(dhierarchy_);

    bonus = 0.0;

    if(master_.is_false(pattern) || master_.is_false(sem))
    return master_.false_value();

    if(master_.is_any(pattern))
    return master_.any_value();

    std::string pattern_s = master_.string_representation(pattern);
    std::string sem_s     = master_.string_representation(sem);

    if(master_.is_any(sem)
       && strchr(pattern_s.c_str(),'\'') == 0)
    {
    // @todo what should be here?

    if(strchr(pattern_s.c_str(),'[') == 0)
    {
        bonus = 1;
        return master_.any_value();
    }
    else
    {
        return master_.false_value();
    }
    }

    return master_.false_value();
/*
    zvalue zsem = dhierarchy_->getDValue(sem_s.c_str());

    if(NULLP(zsem))
    return master_.false_value();

    zvalue zword =
    (master_.is_false(word)
     ? NULL_ZVALUE
     : dhierarchy_->get_zsymbolfactory()->get_symbol(
        master_.string_representation(word).c_str()));

    zvalue zpattern =
    dhierarchy_->getDValue(pattern_s.c_str());

    if(NULLP(zpattern))
    return master_.false_value();

    int score = 0;
    zvalue zr = dhierarchy_->match(zsem, zword, zpattern, score);

    if(NULLP(zr))
    {
    if(score >= -1L)
    {
        if(score == 9999)
        score = 9;
        else
        {
        if(score <= 0 && master_.is_any(sem) &&
           strchr(pattern_s.c_str(),'[') == 0)
            score = 1;
        else
            score = 0;
        }
        bonus = score;
        return sem;
    }

    bonus = 0;
    return master_.false_value();
    }

    if(score <= 0) score = 0;
    else if(score < 7) ++score;
    else if(score < 9999) score = 8;
    else score = 9;

    char* sr = dhierarchy_->getDValueString(zr);
    int ar = master_.from_string(sr);
    delete [] sr;
    bonus = score;
    return ar;
*/
}

